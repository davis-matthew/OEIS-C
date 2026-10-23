/*
 * A197911  Representable by A001045 (Jacobsthal sequence). Complement of
 *          A003158.
 * https://oeis.org/A197911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   merge_with_offset, next_jacobsthal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A197911: a(n) is the n-th smallest integer that can be expressed
 * as a sum of distinct Jacobsthal numbers J_k (k≥1). This implementation
 * generates all subset sums of the distinct Jacobsthal numbers incrementally
 * until it has at least n+1 elements. */

#include <stdlib.h>
#include "../utils/merge_with_offset.h"
#include "../utils/next_jacobsthal.h"

/* Forward declaration of the function required by the build system */
seq_t a(idx_t n);

/* Main function to compute the n-th term of A197911 */
seq_t a(idx_t n)
{
    /* Static state: store all computed subset sums */
    static seq_t *sums = NULL;       /* array of sums */
    static size_t sums_sz = 0;       /* current number of sums stored */
    static int initialized = 0;      /* flag */

    /* Initialize on first call */
    if (!initialized) {
        /* start with 0 (empty sum) */
        sums = (seq_t *)malloc(sizeof(seq_t));
        sums[0] = 0;
        sums_sz = 1;
        initialized = 1;
    }

    /* Ensure we have at least n+1 sums (since n is zero‑based) */
    while (sums_sz <= (size_t)n) {
        seq_t coin = next_jacobsthal();        /* next Jacobsthal number */

        /* Skip duplicates (only 1 appears twice in the sequence) */
        static seq_t last_coin = 0;
        while (coin == last_coin) {
            coin = next_jacobsthal();
        }
        last_coin = coin;

        /* Merge current sums with new sums offset by 'coin' */
        size_t new_sz = 0;
        seq_t *new_sums = merge_with_offset(sums, sums_sz, coin, &new_sz);

        /* Replace old array */
        free(sums);
        sums = new_sums;
        sums_sz = new_sz;
    }

    return sums[(size_t)n];
}
