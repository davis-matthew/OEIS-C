/*
 * A128813  Triangle of coefficients of (x+1)*(x+3)*(x+6)*...*(x+n(n+1)/2).
 * https://oeis.org/A128813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 52, u64 n <= 130,
 *            u128 n <= 295, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the triangle: a(0)=1, a(i,0)=1,
   a(i,j)=i*(i+1)/2*a(i-1,j-1)+a(i-1,j) for 1≤j≤i-1,
   a(i,i)=i*(i+1)/2*a(i-1,i-1), flattened row‑wise. */

#ifdef __cplusplus
#include <vector>
#endif
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector (handles arbitrary‑precision types). */
    if (n == 0) return (seq_t)1;
    size_t count = 0;

    /* Row 0 */
    ++count;                      /* index 0 has been seen */
    size_t prev_len = 1;
    std::vector<seq_t> prev_row(1, (seq_t)1);

    for (size_t i = 1; ; ++i) {
        size_t curr_len = prev_len + 1;
        std::vector<seq_t> curr_row(curr_len);

        seq_t factor = (seq_t)i * ((seq_t)i + 1) / 2;
        curr_row[0] = (seq_t)1;

        for (size_t j = 1; j < curr_len; ++j) {
            seq_t v = factor * prev_row[j - 1];
            if (j < prev_len) v += prev_row[j];
            curr_row[j] = v;
        }

        for (size_t j = 0; j < curr_len; ++j) {
            if (count == n) return curr_row[j];
            ++count;
        }

        prev_row.swap(curr_row);
        prev_len = curr_len;
    }
#else
    /* C implementation (seq_t is a plain unsigned integer). */
    if (n == 0) return (seq_t)1;
    size_t count = 0;

    size_t prev_len = 1;
    seq_t *prev_row = (seq_t *)malloc(sizeof(seq_t));
    if (!prev_row) abort();
    prev_row[0] = (seq_t)1;
    ++count; /* index 0 */

    for (size_t i = 1; ; ++i) {
        size_t curr_len = prev_len + 1;
        seq_t *curr_row = (seq_t *)malloc(curr_len * sizeof(seq_t));
        if (!curr_row) abort();

        seq_t factor = (seq_t)i * ((seq_t)i + 1) / 2;
        curr_row[0] = (seq_t)1;

        for (size_t j = 1; j < curr_len; ++j) {
            seq_t v = factor * prev_row[j - 1];
            if (j < prev_len) v += prev_row[j];
            curr_row[j] = v;
        }

        for (size_t j = 0; j < curr_len; ++j) {
            if (count == n) {
                seq_t result = curr_row[j];
                free(prev_row);
                free(curr_row);
                return result;
            }
            ++count;
        }

        free(prev_row);
        prev_row = curr_row;
        prev_len = curr_len;
    }
#endif
}
