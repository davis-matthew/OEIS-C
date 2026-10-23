/*
 * A103762  a(n) = least k with Sum_{j = n..k} 1/j >= 1.
 * https://oeis.org/A103762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ n such that Σ_{j=n}^{k} 1/j ≥ 1 (offset 1)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Harmonic numbers H[i] = Σ_{j=1}^{i} 1/j, stored in long double */
    static long double *H = NULL;    /* array of harmonic sums */
    static idx_t Hsize = 0;          /* number of elements in H (indices 0 .. Hsize-1) */
    const long double EPS = 1e-18L;   /* tolerance for floating‑point comparison */

    /* Ensure H[0] exists and H[0] = 0.0 */
    if (Hsize == 0) {
        Hsize = 1;
        H = (long double *)malloc(Hsize * sizeof(long double));
        H[0] = 0.0L;
    }

    /* Guarantee that H[n-1] has been computed (needed for target)  */
    if (Hsize <= n - 1) {
        idx_t newsize = n;                 /* at least up to index n-1 */
        if (newsize < 2) newsize = 2;      /* safety for very small n */
        if (newsize < Hsize * 2) newsize = Hsize * 2; /* grow fast */
        H = (long double *)realloc(H, newsize * sizeof(long double));
        for (idx_t i = Hsize; i < newsize; ++i)
            H[i] = H[i - 1] + 1.0L / (long double)i;
        Hsize = newsize;
    }

    /* Target harmonic value to reach */
    long double target = 1.0L + H[n - 1];

    /* Extend H until the tail sum from 1 to k is ≥ target */
    while (H[Hsize - 1] < target) {
        idx_t newsize = Hsize * 2;
        if (newsize <= Hsize) newsize = Hsize + 1;   /* safety for overflow */
        H = (long double *)realloc(H, newsize * sizeof(long double));
        for (idx_t i = Hsize; i < newsize; ++i)
            H[i] = H[i - 1] + 1.0L / (long double)i;
        Hsize = newsize;
    }

    /* Binary search for the minimal k ≥ n with H[k] ≥ target */
    idx_t low = n;
    idx_t high = Hsize - 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (H[mid] >= target - EPS)
            high = mid;
        else
            low = mid + 1;
    }

    return (seq_t)low;
}
