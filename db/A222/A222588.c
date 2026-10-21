/*
 * A222588  Composites of the form 2^n-1 or 2^n+1 that are non-multiples of 3.
 * https://oeis.org/A222588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 6, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates composite numbers of the form
   2^n-1 (n odd) or 2^n+1 (n even) that are not multiples of 3. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"
#include "../utils/pow2.h"

/* Cache of previously computed sequence values. */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static idx_t  a_cache_cnt = 0;

/* The smallest exponent not yet examined.  Start at 2 because
   2^0±1 and 2^1±1 do not satisfy the parity conditions for the
   sequence. */
static idx_t a_next_k = 2;

seq_t a(idx_t n)
{
    /* Expand cache until we have at least n+1 terms. */
    while (a_cache_cnt <= n) {
        /* Compute 2^a_next_k.  pow2 is available in the utils. */
        seq_t power = pow2(a_next_k);
        seq_t val;

        /* Determine whether to subtract or add 1. */
        if (a_next_k & 1)          /* odd exponent → 2^k - 1 */
            val = power - (seq_t)1;
        else                       /* even exponent → 2^k + 1 */
            val = power + (seq_t)1;

        /* Include the value only if it is composite.  is_prime_big
           works for all supported seq_t types.  The parity check
           already guarantees the number is not a multiple of 3. */
        if (!is_prime_big(val)) {
            if (a_cache_cnt == a_cache_cap) {
                size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                a_cache = tmp;
                a_cache_cap = new_cap;
            }
            a_cache[a_cache_cnt++] = val;
        }
        a_next_k++;
    }
    return a_cache[n];
}
