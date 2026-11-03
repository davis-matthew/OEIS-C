/*
 * A172195  Prime numbers for which the absolute difference between the
 *          summation of its digits & the product of its digits is a prime.
 * https://oeis.org/A172195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 925 (2026-11-03)
 * Helpers:   digit_product, digit_sum, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = the n-th prime p such that |sum_of_digits(p) – product_of_digits(p)| is prime
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"
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

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static uint32_t *all_primes = NULL;
    static size_t all_cap   = 0, all_cnt   = 0;

    static seq_t   *good       = NULL;
    static size_t   good_cap   = 0, good_cnt   = 0;

    static uint32_t next_candidate = 2;   /* start with the first prime */

    /* return cached value if already computed */
    if (n <= good_cnt)
        return good[n - 1];

    /* initialise arrays the first time */
    if (!all_primes)
    {
        all_cap = 1024;
        all_primes = (uint32_t *)malloc(all_cap * sizeof(uint32_t));
    }
    if (!good)
    {
        good_cap = 1024;
        good = mem_new(seq_t, good_cap);
    }

    /* generate primes until we have enough good ones */
    while (good_cnt < n)
    {
        /* test primality of next_candidate */
        bool prime = true;
        for (size_t i = 0; i < all_cnt; ++i)
        {
            uint64_t d = all_primes[i];
            if (d * d > next_candidate)
                break;
            if (next_candidate % d == 0)
            {
                prime = false;
                break;
            }
        }

        if (prime)
        {
            /* store prime in all_primes list */
            if (all_cnt == all_cap)
            {
                all_cap <<= 1;
                all_primes = (uint32_t *)realloc(all_primes, all_cap * sizeof(uint32_t));
            }
            all_primes[all_cnt++] = next_candidate;

            /* compute sum, product, difference and check primality */
            seq_t sum  = digit_sum((seq_t)next_candidate);
            seq_t prod = digit_product((seq_t)next_candidate);
            seq_t diff = sum > prod ? sum - prod : prod - sum;

            if (diff >= (seq_t)2 && is_prime_big(diff))
            {
                /* store matching prime */
                if (good_cnt == good_cap)
                {
                    good_cap <<= 1;
                    good = mem_resize(seq_t, good, good_cap);
                }
                good[good_cnt++] = (seq_t)next_candidate;
            }
        }

        /* advance to next candidate
         * after 2 only odd numbers are needed */
        next_candidate = (next_candidate <= 2) ? 3 : next_candidate + 2;
    }

    return good[n - 1];
}
