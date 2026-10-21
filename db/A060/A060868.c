/*
 * A060868  Number of n X n matrices over GF(3) with rank 1.
 * https://oeis.org/A060868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n - 1)^2 / 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is only defined for n >= 1. For n < 1 we return 0. */
    if (n < 1) return 0;

    /* Compute 3^n sequentially to avoid any overflow in the intermediate
       squaring step for 64‑bit or 128‑bit builds. In an arbitrary‑precision
       build this still yields the exact value. */
    seq_t power = 1;
    for (idx_t i = 0; i < n; ++i) {
        power *= (seq_t)3;
    }

    seq_t diff = power - (seq_t)1;
    /* (diff * diff) is always divisible by 2, so integer division is exact. */
    return (diff * diff) / (seq_t)2;
}
