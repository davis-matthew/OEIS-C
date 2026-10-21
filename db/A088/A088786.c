/*
 * A088786  Numbers n which are prime and which when each digit is incremented
 *          by 2 with carries ignored yields another prime p with the same
 *          property.
 * https://oeis.org/A088786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) equals the n‑th prime p such that p, shift2(p), and
 *            shift2(shift2(p)) are all prime, where shift2 increments every
 *            decimal digit of a number by 2 modulo 10 (ignoring carries)
 * Range:     u8 n <= 6, u16 n <= 77, u32 n <= 78, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(idx_t n) is thread‑unsafe but fulfils all
   requirements for sequential calls. */

/*===================================================================*/
/* Common helpers for C and C++                                     */
/*===================================================================*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static inline seq_t shift_digit2(seq_t x)
{
    /* Increment each decimal digit by 2 modulo 10 (carry ignored). */
    if (x == 0) return 2;                 /* 0 never occurs in tests */
    unsigned char buf[128];
    size_t len = 0;
    seq_t t = x;
    while (t > 0) {
        buf[len++] = (unsigned char)(t % 10);
        t /= 10;
    }
    seq_t res = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        unsigned char d  = buf[i];
        unsigned char nd = (d + 2) % 10;
        res = res * 10 + nd;
    }
    return res;
}

/*===================================================================*/
/* C implementation (uses malloc/realloc)                           */
/*===================================================================*/
#ifndef __cplusplus
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = (seq_t)2;

static void grow_cache(size_t new_cap)
{
    if (cache_cap < new_cap) {
        size_t cap = cache_cap ? cache_cap * 2 : 16;
        while (cap < new_cap) cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = cap;
    }
}

static void extend_cache(size_t n)
{
    while (cache_len < n) {
        if (is_prime_big(next_candidate)) {
            seq_t m  = shift_digit2(next_candidate);
            if (is_prime_big(m)) {
                seq_t m2 = shift_digit2(m);
                if (is_prime_big(m2)) {
                    grow_cache(cache_len + 1);
                    cache[cache_len++] = next_candidate;
                }
            }
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, return 0 for n=0 */
    extend_cache((size_t)n);
    return cache[n - 1];
}
#else
/*===================================================================*/
/* C++ implementation (uses std::vector)                             */
/*===================================================================*/
#include <vector>

static std::vector<seq_t> cache_vec;
static seq_t next_candidate_cpp = (seq_t)2;

static void extend_cache_cpp(size_t n)
{
    while (cache_vec.size() < n) {
        if (is_prime_big(next_candidate_cpp)) {
            seq_t m  = shift_digit2(next_candidate_cpp);
            if (is_prime_big(m)) {
                seq_t m2 = shift_digit2(m);
                if (is_prime_big(m2)) {
                    cache_vec.push_back(next_candidate_cpp);
                }
            }
        }
        ++next_candidate_cpp;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, return 0 for n=0 */
    extend_cache_cpp((size_t)n);
    return cache_vec[n - 1];
}
#endif
