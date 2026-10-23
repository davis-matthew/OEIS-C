/*
 * A154507  Nonprimes formed by concatenation of the decimal digits of a
 *          nonprime and its index.
 * https://oeis.org/A154507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(A141468(k), k) filtered to composite values
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   append_candidate, concat_parts, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      A141468(k) is the k‑th nonprime (0,1,4,6,8,9,10,12,…) */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/concat_parts.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* static state for incremental generation */
    static seq_t *cache    = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  next_k    = 1;          /* next index in the nonprime sequence */
    static uint64_t current = 0;          /* current integer being tested for nonprime */

    while (cache_len < n) {
        /* Find next nonprime number */
        while (is_prime(current))
            current++;

        /* current is nonprime; its position is next_k */
        seq_t val = concat_parts(current, next_k);

        if (!is_prime_big(val))
            append_candidate(&cache, &cache_len, &cache_cap, val);

        /* Advance state for next iteration */
        current++;
        next_k++;
    }
    return cache[n - 1];
}
