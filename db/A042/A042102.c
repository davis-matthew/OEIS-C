/*
 * A042102  Numerators of continued fraction convergents to sqrt(575).
 * https://oeis.org/A042102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 48*a(n-2) - a(n-4) (OEIS A042102) Initial values: a(0) =
 *            23 a(1) = 24 a(2) = 1127 a(3) = 1151 The sequence consists of
 *            the numerators of the convergents to sqrt(575)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t: unsigned integer type (up to 128 bits) or arbitrary‑precision integer.
   idx_t: unsigned 64‑bit integer. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)23;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)1127;
    if (n == 3) return (seq_t)1151;

    /* Maintain separate sliding windows for even and odd indices. */
    seq_t e_prev4 = (seq_t)23;   /* a(0) */
    seq_t e_prev2 = (seq_t)1127; /* a(2) */
    seq_t o_prev4 = (seq_t)24;   /* a(1) */
    seq_t o_prev2 = (seq_t)1151; /* a(3) */
    seq_t res;

    for (idx_t i = 4; i <= n; ++i)
    {
        if ((i & 1) == 0)            /* even index */
        {
            res = (seq_t)48 * e_prev2 - e_prev4;
            e_prev4 = e_prev2;
            e_prev2 = res;
        }
        else                          /* odd index */
        {
            res = (seq_t)48 * o_prev2 - o_prev4;
            o_prev4 = o_prev2;
            o_prev2 = res;
        }
    }

    return (n & 1) ? o_prev2 : e_prev2;
}
