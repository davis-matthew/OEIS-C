/*
 * A047227  Numbers that are congruent to {1, 2, 3, 4} mod 6.
 * https://oeis.org/A047227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 2*floor((n-1)/4)
 * Range:     u8 n <= 171, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This enumerates integers congruent to 1,2,3,4 modulo 6. */
seq_t a(idx_t n)
{
    /* n ≥ 1 by OEIS definition; behaviour for n == 0 is undefined. */
    idx_t b = (n - 1) / 4;                /* number of full 4‑term blocks before n */
    seq_t result = (seq_t)n + 2 * (seq_t)b;
    return result;
}
