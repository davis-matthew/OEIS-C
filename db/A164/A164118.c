/*
 * A164118  Expansion of (1 - x^2) * (1 - x^4) * (1 - x^5) / ((1 - x) * (1 -
 *          x^10)) in powers of x.
 * https://oeis.org/A164118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164118: coefficients of (1 - x^2)*(1 - x^4)*(1 - x^5)/((1 - x)*(1 - x^10)) */
/* Recurrence: a(0)=1, a(n+5) = -a(n) for n >= 1  */
/* Explicit O(1) formula: */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    idx_t r = n % (idx_t)5;
    idx_t q = n / (idx_t)5;

    switch (r) {
        case 2: case 3:           /* a(5q+2) = a(5q+3) = 0 */
            return (seq_t)0;

        case 0:                   /* a(5q)   = 2 * (-1)^q   (q >= 1) */
        {
            seq_t s = (q & (idx_t)1) ? (seq_t)-1 : (seq_t)1;
            return (seq_t)2 * s;
        }

        case 1:                   /* a(5q+1) = (-1)^q */
            return (q & (idx_t)1) ? (seq_t)-1 : (seq_t)1;

        case 4:                   /* a(5q+4) = (-1)^(q+1) */
            return (q & (idx_t)1) ? (seq_t)1 : (seq_t)-1;
    }

    /* unreachable */
    return (seq_t)0;
}
