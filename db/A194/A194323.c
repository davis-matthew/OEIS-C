/*
 * A194323  Triangular array: g(n,k)=number of fractional parts (i*sqrt(1/2))
 *          in interval [(k-1)/n, k/n], for 1<=i<=n^2, 1<=k<=n.
 * https://oeis.org/A194323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = g(r, k) where r = smallest integer with r(r+1)/2 ≥ n, k =
 *            n - r(r-1)/2, and g(r, k) = number of i (1 ≤ i ≤ r²) such that
 *            fractional part of i·α lies in [(k-1)/r, k/r), with α = √(1/2)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* use the provided seq_t and idx_t types */
seq_t a(idx_t idx) {
    /* determine the row r and column k (1‑based) for the linear index idx */
    if (idx == 0) return 0;          /* index 0 is outside the OEIS range */
    /* compute r = ceil( ( -1 + sqrt(1+8*idx) ) / 2 ) */
    long double d = sqrtl((long double)8 * (long double)idx + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);        /* floor of the expression */
    if ((r * (r + 1)) / 2 < idx) r++;            /* correct any under‑estimate */

    /* column k (1‑based) and target bucket index (0‑based) */
    idx_t k = idx - (r * (r - 1)) / 2;
    idx_t target = k - 1;

    /* constant α = sqrt(1/2) in long double for better precision */
    static const long double alpha = sqrtl((long double)0.5L);

    /* iterate i = 1 .. r², accumulate the fractional part */
    idx_t i_max = r * r;
    long double f = alpha;              /* fractional part of i*α for i = 1 */
    seq_t count = 0;

    for (idx_t i = 1; i <= i_max; ++i) {
        /* bucket index for current fractional part */
        idx_t bucket = (idx_t)(r * f);   /* r * f < r, truncates like floor */
        if (bucket == target)
            count++;

        /* update f for the next i (adding α and wrapping at 1) */
        f += alpha;
        if (f >= 1.0L)
            f -= 1.0L;
    }
    return count;
}
