/*
 * A057257  Numbers n such that n | 12^n + 11^n + 10^n + 9^n + 8^n + 7^n.
 * https://oeis.org/A057257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   mul_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computed by brute‑force search: 
 * a(n) is the n‑th integer m such that 
 *     m | 12^m + 11^m + 10^m + 9^m + 8^m + 7^m
 * The function stores found values in a static cache so that
 * repeated calls are fast.  Modular arithmetic is done with
 * 64‑bit unsigned integers to avoid overflow when seq_t is
 * smaller (e.g. uint8_t).  The algorithm is O(N·log M) and
 * is well under the time limit for n ≈ 10000.
 */

#include <stdlib.h>   /* for realloc, abort */
#include "../utils/mul_u128.h"

static inline uint64_t pow_u64(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1U) res = mul_u128(res, base, mod);
        base = mul_u128(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* Return the n‑th term of A057257. */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t next_candidate = 1;

    while (cache_len < n) {
        idx_t m = next_candidate;
        uint64_t mod    = m;
        uint64_t sum = 0;

        for (int k = 7; k <= 12; ++k) {
            sum += pow_u64((uint64_t)k, m, mod);
            if (sum >= mod) sum -= mod;   /* keep sum < mod */
        }
        if (sum == 0) {   /* m satisfies the condition */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
