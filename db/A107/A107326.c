/*
 * A107326  Numbers of the form (2^i)*(13^j).
 * https://oeis.org/A107326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 45, u32 n <= 159, u64 n <= 594,
 *            u128 n <= 2294, big any n
 * Helpers:   mul_safe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107326: Numbers of the form 2^i * 13^j, sorted increasingly.
   We generate the sequence using a two‑pointer method analogous to the
   classic Hamming numbers algorithm.  Let a[0] = 1, and maintain two
   indices i and j.  At each step the next term is the smaller of
   a[i]*2 and a[j]*13; if both are equal, increment both indices.
   This guarantees that each number appears exactly once.
   For unsigned integer types we use a 128‑bit intermediate to avoid
   wrap‑around; for arbitrary‑precision types the normal multiplication
   is safe. */
   
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_safe.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used (offset 1), but safe */
    /* allocate array to hold the first n terms */
    seq_t *arr = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!arr) return 0;            /* allocation failure, return 0 as fallback */

    arr[0] = (seq_t)1;
    idx_t i = 0, j = 0;            /* indices for next multiples of 2 and 13 */

    for (idx_t k = 1; k < n; ++k) {
        seq_t next2  = mul_safe(arr[i], (unsigned)2);
        seq_t next13 = mul_safe(arr[j], (unsigned)13);

        if (next2 < next13) {
            arr[k] = next2;
            ++i;
        } else if (next13 < next2) {
            arr[k] = next13;
            ++j;
        } else { /* equal, avoid duplicate */
            arr[k] = next2;
            ++i; ++j;
        }
    }

    seq_t result = arr[n - 1];
    free(arr);
    return result;
}
