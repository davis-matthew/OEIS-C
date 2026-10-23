/*
 * A137680  Triangle read by rows, T(n,k) = T(n-1, k-1) - T(n-k, k-1); with
 *          leftmost term in each row = sum of all previous terms.
 * https://oeis.org/A137680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,1) = sum_{i< n} T(i,j) over all j; T(n,k) = T(n-1,k-1) -
 *            T(n-k,k-1) (k>1) a(n) is the triangle read by rows, a(1)=1
 * Range:     u8 n <= 36, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* data for memoised triangle */
static idx_t max_row   = 0;      /* number of rows already computed */
static idx_t seq_len   = 0;      /* total number of entries stored   */
static size_t capacity = 0;      /* allocated capacity (in entries)  */
static seq_t *seq      = NULL;   /* 0‑based linear array of T values */
static seq_t cum_sum  = 0;       /* sum of all entries produced so far */

/* triangular number T(r) = r*(r+1)/2  (number of entries up to row r) */
static inline idx_t triangular(idx_t r) { return r * (r + 1) / 2; }

/* make sure the first n entries of the triangle are available */
static void ensure_terms(idx_t n)
{
    while (seq_len < n) {
        idx_t r = max_row + 1;   /* next row number, 1‑based */
        idx_t new_len = seq_len + r;
        /* allocate or grow array */
        if (new_len > capacity) {
            size_t new_cap = capacity ? capacity << 1 : 16;
            while (new_cap < (size_t)new_len) new_cap <<= 1;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            capacity = new_cap;
        }

        /* compute current row */
        seq_t row_sum = 0;
        seq_t leftmost = (r == 1) ? (seq_t)1 : cum_sum;
        seq[seq_len] = leftmost;
        row_sum += leftmost;

        /* k = 2 .. r */
        for (idx_t k = 2; k <= r; ++k) {
            /* T(r-1,k-1) */
            idx_t idx1   = triangular(r - 2) + (k - 2);
            seq_t val1   = seq[idx1];

            /* T(r-k,k-1) : defined only if (r-k) >= (k-1) */
            seq_t val2   = 0;
            if ((r - k) >= (k - 1) && r > k) {
                idx_t idx2 = triangular(r - k - 1) + (k - 2);
                val2 = seq[idx2];
            }

            /* signed difference to avoid unsigned underflow */
            __int128 diff = (__int128)(val1) - (__int128)(val2);
            seq_t val = (seq_t)diff;        /* diff should be non‑negative */
            seq[seq_len + (k - 1)] = val;
            row_sum += val;
        }

        /* finish row */
        cum_sum += row_sum;
        seq_len += r;
        max_row = r;
    }
}

/* external linkage as requested */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined for n<1 */
    ensure_terms(n);
    return seq[n - 1];
}
