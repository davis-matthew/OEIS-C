/*
 * A186390  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=6i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186389.
 * https://oeis.org/A186390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor( n(n+1)/12 ) - δ, where δ = 1 if n(n+1)/2 is
 *            divisible by 6, otherwise 0
 * Range:     u8 n <= 22, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the “adjusted joint rank” of the triangular number
   g(n) = n(n+1)/2 with the multiples of 6, placing 6i after the
   triangular number when equal. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1, but guard */
    seq_t nn = (seq_t)n;

    /* Compute the triangular number g(n) = n(n+1)/2 without overflow
       when seq_t is small (≤ uint8).  Divide by 2 first. */
    seq_t T;
    if (nn & 1U) {          /* n is odd: (n+1) is even */
        T = nn * ((nn + 1) / 2);
    } else {                /* n is even: n/2 is integer */
        T = (nn / 2) * (nn + 1);
    }

    /* Number of multiples of 6 up to T. */
    seq_t mult = T / 6;

    /* δ = 1 iff T is divisible by 6. */
    seq_t delta = (T % 6 == 0U) ? 1 : 0;

    return nn + mult - delta;
}
