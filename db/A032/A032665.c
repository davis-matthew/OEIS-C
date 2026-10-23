/*
 * A032665  Digit '2' concatenated with a(n) is a prime.
 * https://oeis.org/A032665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1582, u32 n <= 131, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits, grow_seq, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A032665: a(n) is the n‑th positive integer such that
 * concatenating the digit '2' in front of a(n) yields a prime number. */

#include "../utils/digits.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Return result directly if we already have enough terms */
    if (n == 0)
        return 0;               /* undefined for n < 1 but never called */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Start generating terms from the next candidate */
    seq_t cand = (cache_len == 0) ? (seq_t)1 : cache[cache_len - 1] + (seq_t)1;

    while (cache_len < (size_t)n) {
        /* number of decimal digits of cand */
        idx_t d = digits((idx_t)cand);

        /* 2 concatenated with cand = 2*10^d + cand */
        seq_t pow10d = pow10(d);          /* 10^d */
        seq_t concat = pow10d * (seq_t)2 + cand;

        if (is_prime_big(concat)) {
            /* Store the candidate in the cache */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len] = cand;
            cache_len++;
        }
        cand++; /* next integer */
    }
    return cache[n - 1];
}
