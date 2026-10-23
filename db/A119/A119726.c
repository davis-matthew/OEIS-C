/*
 * A119726  Triangular array read by rows: T(n,1) = T(n,n) = 1, T(n,k) =
 *          4*T(n-1, k-1) + 2*T(n-1, k).
 * https://oeis.org/A119726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 127, u64 n <= 394,
 *            u128 n <= 1409, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: T(n,1)=T(n,n)=1, T(n,k)=4*T(n-1,k-1)+2*T(n-1,k).
   The sequence a(n) is the triangle read in row‑wise order. */

/* Helper allocation that works for both C (seq_t is an unsigned type)
   and C++ (seq_t may be an arbitrary‑precision class). */
#ifdef __cplusplus
static seq_t *seq_alloc(size_t n) { return new seq_t[n]; }
static void seq_free(seq_t *p)      { delete[] p; }
#else
static seq_t *seq_alloc(size_t n) { return (seq_t *)malloc(sizeof(seq_t) * n); }
static void seq_free(seq_t *p)     { free(p); }
#endif

seq_t a(idx_t n)
{
    /* Identify the row r that contains the n‑th term. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Index of the term within its row (0‑based). */
    idx_t prev_total = (r - 1) * r / 2;
    idx_t pos = n - prev_total - 1;

    /* Handle the first row explicitly. */
    if (r == 1) return (seq_t)1;

    /* Build the triangle row by row up to row r. */
    seq_t *prev = seq_alloc(1);
    prev[0] = (seq_t)1;

    for (idx_t rr = 2; rr <= r; ++rr)
    {
        seq_t *curr = seq_alloc((size_t)rr);
        curr[0] = (seq_t)1;
        curr[rr - 1] = (seq_t)1;
        for (idx_t i = 1; i < rr - 1; ++i)
        {
            curr[i] = ((seq_t)4) * prev[i - 1] + ((seq_t)2) * prev[i];
        }
        seq_free(prev);
        prev = curr;
    }

    seq_t result = prev[pos];
    seq_free(prev);
    return result;
}
