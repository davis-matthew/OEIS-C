/*
 * A194667  Triangular array: T(n,k)=[<n*r>+<k*r>], where [ ] = floor, < > =
 *          fractional part, and r = sqrt(3).
 * https://oeis.org/A194667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ⌊{m·√3} + {k·√3}⌋ where m = smallest integer with
 *            m(m+1)/2 ≥ n (row index) k = n – m(m–1)/2 (column index,
 *            1‑based) {x} denotes the fractional part of x and ⌊·⌋ the floor
 * Range:     u8 n <= 4, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes – only int types are required. */
#include <stdbool.h>
#include <stdint.h>

/* ---------- Integer square root of a seq_t (floor) ---------- */
static seq_t floor_sqrt_seq(seq_t x)
{
    seq_t low  = 0;
    seq_t high = x;
    while (low <= high) {
        /* mid = (low + high) / 2, using unsigned arithmetic */
        seq_t mid = low + ((high - low) >> 1);
        seq_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x) low = mid + 1;
        else        high = mid - 1;
    }
    return high;          /* high < low, high is ⌊√x⌋ */
}

/* ---------- Floor of n·√3 computed via integer sqrt ---------- */
static seq_t floor_sqrt_3_times_n2(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = (nn * nn) * 3;        /* 3·n² */
    return floor_sqrt_seq(prod);
}

/* ---------- Find row number for a given linear index n ---------- */
static idx_t row_of(idx_t n)
{
    /* Solve m(m+1)/2 ≥ n, with m integer.  Use binary search on m. */
    idx_t low = 1, high = n;            /* row cannot exceed n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t tri = (mid * (mid + 1)) >> 1;   /* mid*(mid+1)/2 */
        if (tri >= n) high = mid;
        else          low  = mid + 1;
    }
    return low;                         /* minimal row with enough entries */
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Determine row m and column k (1‑based). */
    idx_t row = row_of(n);
    idx_t prev_tri = (row * (row - 1)) >> 1;   /* (row-1)*row/2 */
    idx_t k = n - prev_tri;                    /* column index (1…row) */

    /* Compute floor parts via integer squares. */
    seq_t fn   = floor_sqrt_3_times_n2(row);
    seq_t fk   = floor_sqrt_3_times_n2(k);
    seq_t fsum = floor_sqrt_3_times_n2(row + k);

    /* The value is 0 or 1: floor of fractional sum. */
    return (seq_t)(fsum - fn - fk);
}
