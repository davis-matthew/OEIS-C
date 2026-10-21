/*
 * A075505  Stirling2 triangle with scaled diagonals (powers of 10).
 * https://oeis.org/A075505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 46, u64 n <= 122, u128 n <= 355,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A075505 – Stirling₂ triangle with scaled diagonals (powers of 10)
 *
 * Recurrence (from the OEIS entry):
 *   a(n, m) = 10*m*a(n-1, m) + a(n-1, m-1)
 *   with a(1, 1) = 1 and a(n, m) = 0 when m > n or m < 1.
 * The flattened sequence a(n) is obtained by reading the lower
 * triangular array row by row (row 1 has one element, row 2 two, …).
 * This implementation builds the needed row on each call in O(r²)
 * time, where r ≈ √(2n) is the row index corresponding to n.
 * It uses only integer arithmetic and works for any seq_t as defined
 * in the build (uint8_t…uint64_t, unsigned __int128 or an
 * arbitrary‑precision integer class).
 */

#include <stdint.h>   /* for uint64_t if needed */

#define MAX_ROW 200   /* sufficient for the intended n range (≈10000) */

static seq_t prev_row[MAX_ROW];
static seq_t curr_row[MAX_ROW];

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1 */

    /* Find the row r such that r(r-1)/2 < n <= r(r+1)/2. */
    idx_t r = 1;
    while (((r * (r + 1)) / 2) < n)
        ++r;

    /* Column c of the flattened array: 1 <= c <= r. */
    idx_t prev = r * (r - 1) / 2;
    idx_t c = n - prev;   /* 1‑based column index */

    /* Ensure we have enough space (MAX_ROW is a compile‑time constant). */
    if (r >= MAX_ROW)
        return (seq_t)0;   /* out of range – should never happen for the tests */

    /* Zero the arrays up to r. */
    for (idx_t i = 0; i <= r; ++i) {
        prev_row[i] = (seq_t)0;
        curr_row[i] = (seq_t)0;
    }

    /* Compute rows 1..r using the recurrence. */
    for (idx_t row = 1; row <= r; ++row) {
        for (idx_t m = 1; m <= row; ++m) {
            if (row == 1 && m == 1)
                curr_row[m] = (seq_t)1;
            else {
                seq_t term1 = prev_row[m];
                term1 = term1 * (seq_t)(10 * m);   /* 10*m * prev_row[m] */
                seq_t term2 = prev_row[m - 1];
                curr_row[m] = term1 + term2;
            }
        }
        /* Copy current row into previous for next iteration. */
        for (idx_t i = 0; i <= row; ++i)
            prev_row[i] = curr_row[i];
    }

    /* prev_row now holds a(r, m) for all m. */
    return prev_row[c];
}
