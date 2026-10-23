/*
 * A139251  First differences of toothpicks numbers A139250.
 * https://oeis.org/A139251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 4087, u32 n <= 9923, u64 n <= 9923,
 *            u128 n <= 9923, big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A139251
   a(0) = 0
   a(2^i) = 2^i   for all i ≥ 0
   otherwise, write n = 2^i + j   (0 < j < 2^i)
   then   a(n) = 2*a(j) + a(j+1)
*/

#include <stdlib.h>

#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* Return the n‑th term of the sequence (offset 0). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Allocate an array of seq_t and default‑construct each element. */
    seq_t *arr = seq_alloc_array(n + 1);
    if (!arr)
        return (seq_t)0;                /* unlikely for n ≤ 10000 */

    arr[0] = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Detect a power of two. */
        if ((k & (k - 1)) == 0)
        {
            arr[k] = (seq_t)k;          /* a(2^i) = 2^i */
        }
        else
        {
            /* largest power of two ≤ k */
            idx_t pow = 1;
            while ((pow << 1) <= k)
                pow <<= 1;
            idx_t j = k - pow;          /* 0 < j < pow */
            arr[k] = 2 * arr[j] + arr[j + 1];
        }
    }

    seq_t result = arr[n];
    seq_free_array(arr);

    return result;
}
