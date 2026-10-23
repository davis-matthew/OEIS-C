/*
 * A182013  Triangle of partial sums of Motzkin numbers.
 * https://oeis.org/A182013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{i=k..r} M(i), where M(i) are Motzkin numbers, r and
 *            k are derived from the position n in the flattened triangle: r =
 *            floor(((sqrt(8*n+1)-1)/2)), k = n - r*(r+1)/2
 * Range:     u8 n <= 20, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Find the row number r for a given linear index n (0‑based). */
static idx_t
find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        if (high > (UINT64_MAX >> 1)) break; /* prevent overflow */
        high <<= 1;
    }
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Locate row r and column offset k */
    idx_t r = find_row(n);
    idx_t tri_before = r * (r + 1) / 2;  /* number of elements before row r */
    idx_t k = n - tri_before;           /* 0 <= k <= r */

    /* ----------------------------------------------
       Cache Motzkin numbers and their partial sums.
       ---------------------------------------------- */
    static seq_t *M       = NULL; /* Motzkin numbers M[0..maxR]   */
    static seq_t *prefix  = NULL; /* prefix sums: prefix[i] = Σ_{j=0..i} M[j] */
    static idx_t maxR = 0;        /* highest index currently stored */

    /* First call: allocate for row 0 */
    if (M == NULL) {
        M = (seq_t *)malloc(sizeof(seq_t));
        prefix = (seq_t *)malloc(sizeof(seq_t));
        if (!M || !prefix) return (seq_t)0;   /* allocation failure */
        M[0] = (seq_t)1;
        prefix[0] = M[0];
        maxR = 0;
    }

    /* Extend cache if needed */
    if (r > maxR) {
        size_t newSize = (size_t)(r + 1);
        seq_t *tmpM = (seq_t *)realloc(M, newSize * sizeof(seq_t));
        seq_t *tmpPr = (seq_t *)realloc(prefix, newSize * sizeof(seq_t));
        if (!tmpM || !tmpPr) return (seq_t)0;   /* allocation failure */

        M = tmpM;
        prefix = tmpPr;

        /* Compute Motzkin numbers from maxR+1 up to r */
        for (idx_t i = maxR + 1; i <= r; ++i) {
            if (i == 1) {
                M[1] = (seq_t)1;
                prefix[1] = prefix[0] + M[1];
            } else {
                seq_t a1 = (seq_t)(2 * (idx_t)i + 1) * M[i - 1];
                seq_t a2 = (seq_t)(3 * (idx_t)i - 3) * M[i - 2];
                seq_t numer = a1 + a2;
                seq_t denom = (seq_t)(i + 2);
                M[i] = numer / denom;   /* division is exact */
                prefix[i] = prefix[i - 1] + M[i];
            }
        }
        maxR = r;
    }

    /* Return the partial sum from k to r */
    if (k == 0)
        return prefix[r];
    else
        return prefix[r] - prefix[k - 1];
}
