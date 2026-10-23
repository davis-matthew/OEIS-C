/*
 * A130523  Square array, read by antidiagonals, where row n+1 equals the
 *          partial sums of the previous row after removing the n-th term from
 *          row n for n>=0, with row 0 equal to all 1's.
 * https://oeis.org/A130523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry of an infinite square array read by
 *            antidiagonals. The array is defined as follows: • Row 0 is all
 *            1’s. • For r ≥ 0, row r+1 is obtained from row r by deleting the
 *            element at index r of row r and then taking partial sums of the
 *            remaining sequence. The sequence a(n) is read by an...
 * Range:     u8 n <= 48, u16 n <= 126, u32 n <= 419, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Choose a table size that is large enough for n up to 10 000.
   The maximum row index needed is ⌊(√(8 n+1)−1)/2⌋ < 150, and each
   row may be required up to twice its index during construction.
   A size of 301 gives ample room for all rows and columns. */
#define MAXS 301

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* The sequence and index types are supplied by the build system. */
static seq_t table[MAXS][MAXS];     /* pre‑computed rows */
static bool initialized = false;   /* true after first use of row 0 */
static idx_t built_rows = 0;        /* largest row that has been built */

/* Build row r (r ≥ 1).  It assumes row r‑1 is already built up to
   column MAXS‑1. */
static void build_row(idx_t r)
{
    idx_t prev = r - 1;
    size_t removed = r - 1;                 /* the index that is deleted */

    /* Compute prefix sums of the previous row up to column MAXS‑1. */
    static seq_t prefix[MAXS];
    prefix[0] = table[prev][0];
    for (size_t p = 1; p < MAXS; ++p)
        prefix[p] = prefix[p - 1] + table[prev][p];

    /* Compute the current row values for columns 0 .. MAXS‑2.
       (We need k + 1 < MAXS to avoid accessing prefix[MAXS].) */
    for (size_t k = 0; k + 1 < MAXS; ++k)
    {
        if (k < removed)
            table[r][k] = prefix[k];
        else
            table[r][k] = prefix[k + 1] - table[prev][removed];
    }
}

seq_t a(idx_t n)
{
    /* Initialise row 0 with 1 s on first use. */
    if (!initialized)
    {
        for (size_t col = 0; col < MAXS; ++col)
            table[0][col] = (seq_t)1;
        initialized = true;
    }

    /* Convert the linear index n into (row,inColumn) by reading the
       array along antidiagonals.  The k‑th antidiagonal contains
       indices (0,k),(1,k−1),…,(k,0).  For a given n we find the
       smallest k with T(k+1) > n and then obtain offset=n−T(k).  */
    idx_t k = 0;
    while ((k + 1) * (k + 2) / 2 <= n)
        ++k;
    idx_t offset = n - k * (k + 1) / 2;

    /* Coordinates: i = k − offset (row),   j = offset (column). */
    size_t i = (size_t)(k - offset);
    size_t j = (size_t)offset;

    /* Ensure all rows up to i have been built. */
    while (built_rows < i)
        build_row(++built_rows);

    return table[i][j];
}
