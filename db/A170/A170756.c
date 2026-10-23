/*
 * A170756  Expansion of g.f.: (1+x)/(1-36*x).
 * https://oeis.org/A170756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n >= 1, a(n) = 36^n + 36^(n-1) = 37 * 36^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }

    /* Compute 36^(n-1) */
    seq_t power = 1;
    for (idx_t i = 1; i < n; ++i) {
        power *= 36;      /* multiply by 36 for each step */
    }

    /* a(n) = 37 * 36^(n-1) */
    return power * 37;
}
