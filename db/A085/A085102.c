/*
 * A085102  Go on adding the divisors of n starting from n in decreasing order
 *          until one gets a prime. a(n) = this prime, or 0 if no prime is
 *          obtained.
 * https://oeis.org/A085102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 93 terms through n = 855 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085102: start from n, repeatedly add its proper divisors in
 * decreasing order until a prime is reached.  a(n) = that prime, or 0
 * if no prime appears in the process.  The first term (a(1)) is 0 as
 * defined by the OEIS entry.  This implementation uses the helper
 * functions int_sqrt (to obtain ⌊√n⌋) and is_prime_big (to test primality
 * of a seq_t value).  The divisors are emitted in correct order by
 * adding the larger partner first while storing the smaller partner in
 * a small dynamic array, which is then traversed in reverse after all
 * larger divisors have been processed. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isqrt.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;                /* a(1) = 0 by definition */

    seq_t sum = (seq_t)n;                /* start with n itself */

    if (is_prime_big(sum))
        return sum;                     /* if n is prime, done */

    idx_t limit = isqrt(n);           /* floor(sqrt(n)) */

    /* Dynamic array for the smaller divisors (ascending order). */
    size_t cap = 128;
    idx_t *smallers = (idx_t *)malloc(cap * sizeof(idx_t));
    if (smallers == NULL)
        return (seq_t)0;                 /* allocation failure → 0 */

    size_t cnt = 0;                      /* number of small divisors stored */

    for (idx_t d = 2; d <= limit; ++d) {
        if (n % d == 0) {
            idx_t larger = n / d;
            sum += (seq_t)larger;
            if (is_prime_big(sum)) {
                free(smallers);
                return sum;
            }

            /* Store the smaller divisor for later; avoid duplicates
             * when d == larger (perfect square). */
            if (larger != d) {
                if (cnt == cap) {
                    cap <<= 1;
                    idx_t *p = (idx_t *)realloc(smallers, cap * sizeof(idx_t));
                    if (!p) {
                        free(smallers);
                        return (seq_t)0;
                    }
                    smallers = p;
                }
                smallers[cnt++] = d;
            }
        }
    }

    /* Add the stored smaller divisors in reverse order (descending). */
    for (size_t i = cnt; i > 0; --i) {
        sum += (seq_t)smallers[i - 1];
        if (is_prime_big(sum)) {
            free(smallers);
            return sum;
        }
    }

    /* Finally add the divisor 1. */
    sum += (seq_t)1;
    if (is_prime_big(sum))
        sum = sum;   /* prime found */

    free(smallers);
    return is_prime_big(sum) ? sum : (seq_t)0;
}
