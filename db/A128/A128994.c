/*
 * A128994  First of three consecutive pairwise coprime numbers such that the
 *          product of any two plus the third is a prime.
 * https://oeis.org/A128994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 24, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   gcd, is_prime_big, mul_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A128994: first of three consecutive pairwise coprime numbers m
   such that the products of any two plus the third are prime.
   a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=6, ...
   The algorithm searches from the last computed value forward,
   keeping a cache so that calls with arbitrary n order are fast.
*/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"
#include "../utils/mul_add.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* ensure the cache is large enough for 'needed' elements */
static void ensure_cache_capacity(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();          /* out of memory */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* main function required by the framework */
seq_t a(idx_t n) {
    if (n == 0) return 0;        /* undefined, but keep safety */
    if (n > cache_size) {
        seq_t next_candidate = (cache_size ? cache[cache_size - 1] + 1 : 1);
        for (idx_t idx = cache_size + 1; idx <= n; ++idx) {
            seq_t m = next_candidate;
            for (;;) {
                /* find the next coprime number x > m */
                seq_t x = m + 1;
                while ((gcd(m, x) != (seq_t)1)) ++x;

                /* find the next coprime number y > x coprime to both m and x */
                seq_t y = x + 1;
                while (!((gcd(m, y) == (seq_t)1) && (gcd(x, y) == (seq_t)1))) ++y;

                /* check the primality conditions */
                seq_t t1 = mul_add(m, x, y);
                seq_t t2 = mul_add(m, y, x);
                seq_t t3 = mul_add(x, y, m);
                if (is_prime_big(t1) && is_prime_big(t2) && is_prime_big(t3))
                    break;          /* m is a valid term */
                ++m;                 /* otherwise try next m */
            }

            /* store the found term and prepare candidate for next search */
            ensure_cache_capacity(idx);
            cache[cache_size++] = m;
            next_candidate = m + 1;
        }
    }
    return cache[n - 1];
}
