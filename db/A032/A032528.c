/*
 * A032528  Concentric hexagonal numbers: floor(3*n^2/2).
 * https://oeis.org/A032528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3*n^2/2)
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = n;
    /* 3*x*x/2 truncates toward zero, which equals the floor for non‑negative n */
    return 3 * x * x / 2;
}
