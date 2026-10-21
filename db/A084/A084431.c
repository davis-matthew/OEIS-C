/*
 * A084431  Expansion of g.f. (1 + 6*x + 5*x^2)/((1-2*x)*(1+2*x)).
 * https://oeis.org/A084431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084431: Expansion of (1 + 6*x + 5*x^2)/((1-2*x)*(1+2*x))
   Formula (OEIS):  a(n) = (-3*(-2)^n + 21*2^n)/8
   Recurrence    :  a(0)=1, a(1)=6, a(2)=9, a(n)=4*a(n-2)  (n≥3) */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)6;
    }
    if (n == 2) {
        return (seq_t)9;
    }

    /* For n≥3 use the recurrence a(n)=4*a(n-2).  Separate even/odd
       indices to avoid uninitialized values. */
    seq_t val;
    idx_t steps;

    if ((n & 1) == 0) {            /* even n */
        val = (seq_t)9;             /* a(2) */
        steps = (n - 2) >> 1;       /* (n-2)/2 */
    } else {                        /* odd n */
        val = (seq_t)6;             /* a(1) */
        steps = (n - 1) >> 1;       /* (n-1)/2 */
    }

    for (idx_t i = 0; i < steps; ++i) {
        val *= (seq_t)4;
    }
    return val;
}
