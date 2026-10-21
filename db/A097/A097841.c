/*
 * A097841  First differences of Chebyshev polynomials S(n,83) = A097839(n)
 *          with Diophantine property.
 * https://oeis.org/A097841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 83*a(n-1) - a(n-2), with a(0)=1, a(1)=82
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence is derived from the generating function
   (1-x)/(1-83*x+x^2) of A097841. */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)82;

    /* Compute iteratively up to n. */
    seq_t prevprev = (seq_t)1;  /* a(0) */
    seq_t prev     = (seq_t)82; /* a(1) */
    seq_t cur      = prev;      /* placeholder */

    /* constant 83 cast to seq_t */
    const seq_t c83 = (seq_t)83;

    idx_t i;
    for (i = 2; i <= n; ++i) {
        cur = c83 * prev - prevprev;
        prevprev = prev;
        prev = cur;
    }
    return cur;
}
