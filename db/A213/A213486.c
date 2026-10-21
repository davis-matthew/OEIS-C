/*
 * A213486  Number of (w,x,y) with all terms in {0,...,n} and
 *          |w-x|+|x-y|+|y-w| > w+x+y.
 * https://oeis.org/A213486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * Σ_{k=0}^{⌊(n-1)/2⌋} (-1)^k * binom(n-2k+2, 3) where
 *            binom(m,3) = m*(m-1)*(m-2)/6. This matches the generating
 *            function 3*x/((1-x)^4*(1+x^2))
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    /* handle the first six terms explicitly to avoid unsigned underflow */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)27;
    if (n == 4) return (seq_t)48;
    if (n == 5) return (seq_t)78;

    long long sum = 0;           /* intermediate accumulator (signed 64‑bit) */
    idx_t limit = n - 1;         /* upper bound for k in the sum */

    for (idx_t k = 0; 2 * k <= limit; ++k) {
        /* m = n - 2k + 2 is the argument of the binomial coefficient */
        idx_t m = n - 2 * k + 2;

        /* compute binom(m,3) = m*(m-1)*(m-2)/6 safely using 128‑bit arithmetic */
        unsigned __int128 tmp = (unsigned __int128)m * (m - 1) * (m - 2) / 6;
        long long term = (long long)tmp;

        /* accumulate with alternating sign */
        if (k & 1) sum -= term;
        else       sum += term;
    }

    sum *= 3;                    /* multiply by 3 as per the formula */

    return (seq_t)sum;           /* cast back to the required seq_t type */
}
