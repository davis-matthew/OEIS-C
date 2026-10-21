/*
 * A093998  Number of partitions of n with an even number of distinct
 *          Fibonacci parts.
 * https://oeis.org/A093998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (∏_{k≥2} (1 + x^{F_k}) + ∏_{k≥2} (1 - x^{F_k}))/2
 * Range:     u8 n <= 104, u16 n <= 8813, u32 n <= 8813, u64 n <= 8813,
 *            u128 n <= 8813, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts subsets of distinct Fibonacci numbers (starting with 1,2,3,5,…) that sum to n
   and contain an even number of elements. */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    size_t len = (size_t)n + 1;
    seq_t *even  = alloc_seq_array(len);
    seq_t *odd   = alloc_seq_array(len);
    seq_t *even2 = alloc_seq_array(len);
    seq_t *odd2  = alloc_seq_array(len);

    for (size_t i = 0; i < len; ++i) {
        even[i] = 0;
        odd[i]  = 0;
    }
    even[0] = 1;

    seq_t f1 = 1;  /* F_2 */
    seq_t f2 = 2;  /* F_3 */

    for (seq_t f = f1; f <= n; ) {
        for (size_t i = 0; i < len; ++i) {
            even2[i] = even[i];
            odd2[i]  = odd[i];
        }

        size_t sf = (size_t)f;
        for (size_t s = sf; s < len; ++s) {
            even2[s] += odd[s - sf];
            odd2[s]  += even[s - sf];
        }

        seq_t *tmp = even; even = even2; even2 = tmp;
        tmp = odd; odd = odd2; odd2 = tmp;

        seq_t next = f1 + f2;
        f1 = f2;
        f2 = next;
        f = f1;
    }

    seq_t result = even[(size_t)n];

    free_seq_array(even);
    free_seq_array(odd);
    free_seq_array(even2);
    free_seq_array(odd2);

    return result;
}
