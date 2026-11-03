/*
 * A115261  Prime numbers such that the absolute difference of the sum of
 *          their digits in odd positions and the sum of their digits in even
 *          positions is also a prime.
 * https://oeis.org/A115261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n-th prime such that the absolute difference of the
 *            sum of its digits in odd positions and the sum in even positions
 *            is also a prime
 * Range:     u8 n <= 31, u16 n <= 2901, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 8750 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple incremental search for primes
   and memorises the sequence in a static array.  The test for primality
   of a number is delegated to the helper from the utils directory:
   is_prime_big.  Digits are processed from right to left; the
   resulting absolute difference is evaluated as a prime using the same
   helper.  The function works for any supported seq_t and is
   independent of the order of calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"
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

seq_t a(idx_t n) {
    /* The OEIS offset is 1; for n == 0 we return 0 (undefined) */
    if (n == 0) return 0;

    /* static memoisation state */
    static seq_t *cache = NULL;      /* dynamic array of qualifying primes */
    static size_t cap = 0;           /* allocated capacity */
    static size_t used = 0;          /* number of elements stored */
    static seq_t last_prime = 1;     /* start just before the first prime (2) */

    /* grow storage if necessary */
    if (cap < (size_t)n) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap < (size_t)n) newcap = n;
        seq_t *tmp = mem_resize(seq_t, cache, (newcap * sizeof *cache) / sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cap = newcap;
    }

    /* search for new qualifying primes until we have n terms */
    while (used < (size_t)n) {
        /* advance to next prime */
        do { ++last_prime; } while (!is_prime_big(last_prime));

        /* compute sum of digits in odd and even positions (from right) */
        seq_t tmp = last_prime;
        int odd_sum  = 0;
        int even_sum = 0;
        int pos = 1;               /* 1‑based from the least‑significant digit */
        while (tmp != 0) {
            int digit = (int)(tmp % 10);
            if (pos & 1) odd_sum += digit; else even_sum += digit;
            ++pos;
            tmp /= 10;
        }

        /* absolute difference */
        int diff = odd_sum - even_sum;
        if (diff < 0) diff = -diff;

        /* check if the difference is a prime (>1) */
        if (diff > 1 && is_prime_big((seq_t)diff)) {
            cache[used++] = last_prime;
        }
    }

    return cache[n - 1];
}
