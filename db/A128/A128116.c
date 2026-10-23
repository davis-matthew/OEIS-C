/*
 * A128116  A128064 * A122432 (unsigned).
 * https://oeis.org/A128116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = P(r) - (j-1)*(2r) + (j-1)*j/2 with r = smallest integer
 *            such that r(r+1)/2 >= n, j = n - r(r-1)/2 (column index). P(r) =
 *            r*(3r-1)/2 (pentagonal numbers)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: find row r such that r*(r+1)/2 >= n */
static idx_t _row_of(idx_t n)
{
    idx_t r = 1;
    /* The loop runs at most O(sqrt(n)) iterations, which is fine
       for the practical size of the sequence (n up to ~10^5 in tests). */
    while ( ( (seq_t)r * (r + 1) / 2 ) < n )
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n<1, but return 0 to avoid UB */
        return (seq_t)0;

    idx_t r = _row_of(n);
    /* offset where this row starts in the linear list:
       offset = (r-1)*r/2 ; j = n - offset (1-based column) */
    idx_t offset = (r - 1) * r / 2;
    idx_t j = n - offset;          /* column index, 1 <= j <= r */

    seq_t r_seq = (seq_t)r;
    seq_t j_seq = (seq_t)j;
    seq_t jm1  = j_seq - 1;        /* j-1, safe because j >= 1 */

    /* Compute the formula term by term to avoid overflow */
    seq_t pent = r_seq * ((seq_t)3 * r_seq - 1) / 2;          /* P(r) */
    seq_t part1 = pent;
    seq_t part2 = jm1 * (2 * r_seq);                         /* (j-1)*(2r) */
    seq_t part3 = jm1 * j_seq / 2;                            /* (j-1)*j/2 */

    return part1 - part2 + part3;
}
