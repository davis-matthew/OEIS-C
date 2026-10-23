/*
 * A008756  Expansion of (1+x^13)/((1-x)*(1-x^2)*(1-x^3)).
 * https://oeis.org/A008756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = partitions of n into parts 1, 2, 3 + (partitions of n-13
 *            into parts 1, 2, 3, if n ≥ 13)
 * Range:     u8 n <= 42, u16 n <= 630, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Memoised array of p(k) = number of partitions of k using 1,2,3.  */
    static seq_t *P = NULL;
    static idx_t Psize = 0;

    /* Grow the array if needed.  */
    if (n + 1 > Psize) {
        idx_t new_size = n + 1;
        seq_t *tmp = alloc_seq_array(new_size);
        /* If we already had memory, free it.  */
        if (P && Psize)
            free_seq_array(P);
        P = tmp;

        /* Initialise to zero.  */
        for (idx_t i = 0; i < new_size; ++i)
            P[i] = 0;
        P[0] = 1;          /* Only the empty partition for 0. */

        /* Classic coin‑change DP for coins 1,2,3.  */
        for (idx_t coin = 1; coin <= 3; ++coin) {
            for (idx_t j = coin; j < new_size; ++j)
                P[j] += P[j - coin];
        }
        Psize = new_size;
    }

    seq_t result = P[n];
    if (n >= 13)
        result += P[n - 13];
    return result;
}
