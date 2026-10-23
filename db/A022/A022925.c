/*
 * A022925  Number of 5^m between 2^n and 2^(n+1).
 * https://oeis.org/A022925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)*log_5(2)) - floor(n*log_5(2))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of powers of 5 that lie strictly between 2^n and 2^(n+1). */

#include <math.h>

seq_t a(idx_t n)
{
    /*  log_5(2)  is a fixed irrational number.
        Its value is approximated with long double precision,
        enough for all n up to at least 10 000 (which is far beyond
        the range of typical test cases). */
    static const long double LOG5_2 = logl(2.0L) / logl(5.0L);

    /* Compute the two floor terms using long double arithmetic.
       The casts to long long are safe because the results are at
       most about 0.43 *n, so for n <= 10 000 they are < 5 000.
       Casting from long long to seq_t uses the integer constructor
       provided by the arbitrary‑precision implementation. */
    long double v0 = (long double)n * LOG5_2;
    long double v1 = (long double)(n + 1) * LOG5_2;

    long long f0 = (long long)floorl(v0);
    long long f1 = (long long)floorl(v1);

    return (seq_t)(f1 - f0);
}
