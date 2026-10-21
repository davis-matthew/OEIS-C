/*
 * A086604  a(n) = 2^(n-3)n(9n^2-9n+4).
 * https://oeis.org/A086604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-3)*n*(9n^2-9n+4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for all non‑negative integer n. */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;                /* promote n to seq_t */
    /* common polynomial part: n*(9n^2-9n+4) */
    seq_t poly = ns * ((seq_t)9 * ns * ns
                       - (seq_t)9 * ns
                       + (seq_t)4);

    if (n <= 3) {
        /* for n <= 3 we need to divide by 2^(3-n) */
        return poly >> (3 - n);        /* shift right by 0..3 bits */
    } else {
        /* for n > 3 we multiply by 2^(n-3) safely,
           without shifting by a too large amount */
        idx_t shift = n - 3;
        for (idx_t i = 0; i < shift; ++i)
            poly <<= 1;                /* shift left by 1, repeated */
        return poly;
    }
}
