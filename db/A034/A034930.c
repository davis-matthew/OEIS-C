/*
 * A034930  Triangle, read by rows, formed by reading Pascal's triangle
 *          (A007318) mod 8.
 * https://oeis.org/A034930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n+1,k) = (T(n,k) + T(n,k-1)) mod 8 Triangle read by rows,
 *            formed by reading Pascal's triangle mod 8 a(0) = 1, a(1) = 1,
 *            a(2) = 1, a(3) = 1, … (see OEIS entry A034930)
 * Range:     u8 n <= 7380, u16 n <= 7380, u32 n <= 7380, u64 n <= 7380,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find row index r such that n lies in row r.
       The total number of integers written up to (and including) row r
       is (r+1)(r+2)/2.  Therefore r is the smallest integer with
       (r+1)(r+2)/2 > n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Allocate one row of size r+1.  Use new[] in C++ and malloc in C. */
#ifdef __cplusplus
    seq_t *row = new seq_t[r + 1];
#else
    seq_t *row = (seq_t *)malloc(sizeof(seq_t) * (r + 1));
#endif
    if (!row)
        return 0; /* allocation failure – unlikely for the given limits */

    /* Initialise the first element of the row. */
    for (idx_t i = 0; i <= r; ++i)
        row[i] = 0;
    row[0] = 1;

    /* Build rows 1 … r iteratively in-place. */
    for (idx_t i = 1; i <= r; ++i) {
        row[i] = 1;                     /* new element at the end */
        for (idx_t j = i - 1; j > 0; --j) {
            seq_t sum = row[j] + row[j - 1];
            if (sum >= (seq_t)8)
                sum -= (seq_t)8;          /* mod 8 */
            row[j] = sum;
        }
        /* row[0] remains 1 */
    }

    seq_t result = row[k];

    /* Free memory.  Use delete[] in C++ and free() in C. */
#ifdef __cplusplus
    delete[] row;
#else
    free(row);
#endif

    return result;
}
