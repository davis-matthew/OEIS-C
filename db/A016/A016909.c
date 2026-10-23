/*
 * A016909  Expansion of 1/((1-3*x)*(1-4*x)*(1-9*x)).
 * https://oeis.org/A016909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 75*a(n-2) + 108*a(n-3) with initial values
 *            a(0) = 1, a(1) = 16, a(2) = 181. This follows from the expansion
 *            of 1/((1-3*x)*(1-4*x)*(1-9*x))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)181;

    /* Iterative computation using the homogeneous recurrence. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)16;  /* a(1) */
    seq_t a2 = (seq_t)181; /* a(2) */
    seq_t a3;

    for (idx_t i = 3; i <= n; ++i) {
        a3 = 16 * a2 - 75 * a1 + 108 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
