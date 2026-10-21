/*
 * A064326  Generalized Catalan numbers C(-4; n).
 * https://oeis.org/A064326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{m=0..n-1} (n-m)*binomial(n-1+m, m)*(-4)^m / n, with
 *            a(0)=1. The implementation uses an exact integer arithmetic
 *            routine. For small seq_t types (≤64 bits) it employs __int128
 *            intermediate variables to avoid overflow; for larger seq_t types
 *            (e.g. arbitrary‑precision) it uses seq_t directly
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 18, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return (seq_t)1;

    /* If seq_t is 64 bits or smaller, use __int128 for all
       intermediate calculations to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        __int128 sum = 0;         /* running sum */
        __int128 pow = 1;         /* (-4)^m  */
        __int128 bin = 1;         /* binomial(n-1+m, m) */

        for (idx_t m = 0; m < n; ++m) {
            /* term = pow * bin * (n-m) / n  and add to sum */
            __int128 term = pow * bin * ((__int128)n - m);
            term /= (__int128)n;
            sum += term;

            /* update bin to C(n-1+(m+1), m+1) = bin * (n+m) / (m+1) */
            if (m + 1 == n)
                break;           /* no further terms needed */
            __int128 num = (__int128)n + m; /* n + m  */
            __int128 den = m + 1;
            bin = bin * num / den;

            /* next power of (-4) */
            pow *= (__int128)-4;
        }
        return (seq_t)sum;
    }

    /* For seq_t with more than 64 bits (arbitrary‑precision case)
       we perform all arithmetic in seq_t, trusting its operator
       overloads for big integers. */
    seq_t sum = 0;
    seq_t pow = 1;       /* (-4)^m  */
    seq_t bin = 1;       /* binomial(n-1+m, m) */

    for (idx_t m = 0; m < n; ++m) {
        seq_t term = pow * bin * ((seq_t)n - m);
        term /= (seq_t)n;          /* exact division */
        sum += term;

        if (m + 1 == n)
            break;               /* no further terms needed */
        seq_t num = (seq_t)n + m;
        seq_t den = (seq_t)(m + 1);
        bin = bin * num / den;    /* exact division */
        pow *= (seq_t)-4;
    }
    return sum;
}
