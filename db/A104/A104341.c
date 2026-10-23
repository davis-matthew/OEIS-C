/*
 * A104341  Numbers n such that n -(digital reversal of n) = 9.
 * https://oeis.org/A104341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   a(n) = 10 + 11*(n-1) for 1 ≤ n ≤ 9; a(n) = 0 otherwise (sequence
 *            of two‑digit numbers where tens digit = ones digit + 1). All
 *            other n do not satisfy n - rev(n) = 9
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; any n <= 9 yields a valid term. */
    if (n < 1 || n > 9)   /* out of range – sequence ends */
        return (seq_t)0;

    /* Compute 10 + 11*(n-1) avoiding overflow beyond 64‑bit if seq_t is small */
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)10 + (seq_t)11 * (nn - (seq_t)1);
    return val;
}
