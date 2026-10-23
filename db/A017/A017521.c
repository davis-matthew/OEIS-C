/*
 * A017521  Expansion of 1/((1-3*x)*(1-5*x)*(1-8*x)).
 * https://oeis.org/A017521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 79*a(n-2) + 120*a(n-3) Initial values:
 *            a(0)=1, a(1)=16, a(2)=177 This is derived from the generating
 *            function 1/((1-3x)(1-5x)(1-8x))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three initial terms directly. */
    if (n == 0) return 1;
    if (n == 1) return 16;
    if (n == 2) return 177;

    /* Variables to hold the three preceding terms: a_{i-3}, a_{i-2}, a_{i-1}. */
    seq_t a0 = 1;
    seq_t a1 = 16;
    seq_t a2 = 177;
    seq_t next;

    /* Iteratively compute terms up to a(n). */
    for (idx_t i = 3; i <= n; ++i) {
        /* Using (16*a2 + 120*a0) - 79*a1 ensures the intermediate
           subtraction does not underflow when seq_t is unsigned. */
        next = (16 * a2 + 120 * a0) - 79 * a1;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
