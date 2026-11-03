/*
 * A125523  Democratic primes: primes such that the left half of the prime is
 *          prime and the right half is not.
 * https://oeis.org/A125523
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth Democratic prime (a(2)=29)
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 50 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n < 2)
        return (seq_t)0;                 /* undefined below the offset */

    /* Static cache of Democratic primes */
    static seq_t *cache        = NULL;  /* array of found primes          */
    static size_t cache_used   = 0;     /* number of primes stored        */
    static size_t cache_cap    = 0;     /* allocated capacity             */
    static idx_t  next_candidate = 2;   /* next integer to test           */

    /* Generate primes until we have (n‑1) Democratic primes
       (since a(2) is the first term). */
    while (cache_used < (size_t)(n - 1)) {
        /* Resize cache if necessary */
        if (cache_used >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)
                return (seq_t)0;          /* allocation failure - unlikely   */
            cache    = tmp;
            cache_cap = new_cap;
        }

        /* Test the current candidate for primality */
        if (is_prime_big((seq_t)next_candidate)) {
            seq_t p   = (seq_t)next_candidate;
            int   len = num_digits(p);
            int   half = len / 2;

            /* Left half: first floor(len/2) digits */
            seq_t left_den = pow10((idx_t)(len - half));
            seq_t left     = p / left_den;

            /* Right half: last floor(len/2) digits */
            seq_t right    = p % pow10((idx_t)half);

            if (is_prime_big(left) && !is_prime_big(right)) {
                cache[cache_used++] = p;
            }
        }
        ++next_candidate;
    }

    return cache[n - 2];
}
