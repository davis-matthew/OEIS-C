/*
 * A026931  Triangular array T read by rows: T(n,k) = number of partitions of
 *          n into an odd number of parts, each <= k.
 * https://oeis.org/A026931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = ceil((sqrt(8*n+1)-1)/2), * k = n -
 *            (r-1)*r/2, * T(n,k) = number of partitions of n into an odd
 *            number of parts, each part ≤ k
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static inline idx_t row_of(idx_t n)
{
    idx_t r = 0;
    while (r * (r + 1) / 2 < n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t r = row_of(n);
    idx_t k = n - ((r - 1) * r / 2);
    if (k > r)
        k = r;

    seq_t *even = alloc_seq_array(r + 1);
    seq_t *odd  = alloc_seq_array(r + 1);
    if (!even || !odd) {
        free_seq_array(even);
        free_seq_array(odd);
        return (seq_t)0;
    }

    for (idx_t i = 0; i <= r; ++i) {
        even[i] = (seq_t)0;
        odd[i]  = (seq_t)0;
    }
    even[0] = (seq_t)1;  /* zero parts is even */

    for (idx_t part = 1; part <= k; ++part) {
        for (idx_t sum = part; sum <= r; ++sum) {
            seq_t newOdd  = odd[sum] + even[sum - part];
            seq_t newEven = even[sum] + odd[sum - part];
            odd[sum]  = newOdd;
            even[sum] = newEven;
        }
    }

    seq_t result = odd[r];

    free_seq_array(even);
    free_seq_array(odd);

    return result;
}
