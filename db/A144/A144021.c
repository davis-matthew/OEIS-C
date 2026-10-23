/*
 * A144021  Eigentriangle by rows, T(n,k) = A000034(n-k+1)*A105476(k-1).
 * https://oeis.org/A144021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000034(n-k+1) * A105476(k-1) where k = n - (r-1)r/2 and
 *            r is the smallest integer with r(r+1)/2 ≥ n (OEIS offset 1).
 *            A000034(j) is 1 for odd j and 2 for even j
 * Range:     u8 n <= 43, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: A105476 satisfies the recurrence:
        a[0] = 1, a[1] = 1
        a[n] = Σ_{m=0}^{n-1} ( (m+1 odd ? 1 : 2) * a[n-1-m] )  for n ≥ 2
   This recurrence is used to generate the required values independently
   of n. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find row r such that previous triangular sum < n ≤ current triangular sum. */
    idx_t r = 1;
    idx_t tri = 1;               /* r*(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;                 /* add next integer to the triangular sum */
    }
    idx_t prev_tri = tri - r;    /* sum up to row r-1 : (r-1)*r/2 */
    idx_t k = n - prev_tri;      /* 1‑based column index in this row */
    idx_t need = k - 1;          /* index into A105476 for the right factor */

    /* Static cache for A105476 values up to a maximum index. */
    static seq_t a105[256];
    static idx_t a105_max = 1;   /* we already know a105[0] and a105[1] */
    static bool initialized = false;

    if (!initialized) {
        a105[0] = (seq_t)1;
        a105[1] = (seq_t)1;
        a105_max = 1;
        initialized = true;
    }

    /* Extend the cache if necessary. */
    if (need > a105_max) {
        for (idx_t i = a105_max + 1; i <= need; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t m = 0; m < i; ++m) {
                seq_t coeff = (((m + 1) & 1) ? (seq_t)1 : (seq_t)2);
                sum += coeff * a105[i - 1 - m];
            }
            a105[i] = sum;
        }
        a105_max = need;
    }

    /* Compute the A000034 factor for position (n-k+1). */
    idx_t raw = r - k + 1;
    seq_t coeff = (((raw & 1) ? (seq_t)1 : (seq_t)2));

    return coeff * a105[need];
}
