/*
 * A035505  Active part of Kimberling's expulsion array as a triangular array.
 * https://oeis.org/A035505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, nice, easy
 * Formula:   a(n) = K(A000194(n)+2, A074294(n)), where K(i,j) is defined
 *            recursively: if j >= 2*i-3 → K = i + j - 1 else if j even → K =
 *            K(i-1, i-(j+2)/2) else → K = K(i-1, i+(j-1)/2) The sequence
 *            enumerates the active part of Kimberling's expulsion array, row
 *            by row, starting at i = 3 with j = 1..(2i-4)
 * Range:     u8 n <= 7308, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9402 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute K(i,j) iteratively to avoid recursion depth. */
static seq_t K(idx_t i, idx_t j)
{
    while (1) {
        if (j >= 2*i - 3) {             /* base case */
            return (seq_t)i + (seq_t)j - 1;
        }
        /* non‑base case – descend one row */
        if ((j & 1) == 0) {               /* j even */
            j = i - ((j + 2) / 2);
        } else {                          /* j odd */
            j = i + ((j - 1) / 2);
        }
        --i;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Undefined for n < 1, handle gracefully */

    idx_t i = 3;
    /* Find the smallest i with cumulative(i) >= n.
       cumulative(i) = (i-1)*(i-2) – total elements up to row i.   */
    while ((unsigned __int128)(i - 1) * (i - 2) < n) {
        ++i;
    }

    idx_t prev = (unsigned __int128)(i - 2) * (i - 3);  /* elements before row i */
    idx_t j = n - prev;                                 /* j is 1‑based within row */

    return K(i, j);
}
