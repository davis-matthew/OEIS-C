/*
 * A166730  Positive integers with English names ending in "y".
 * https://oeis.org/A166730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = 100 * floor((n-1)/8) + 10 * ((n-1) mod 8 + 2)
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This yields the n-th positive integer whose English name ends in "y",
   with a(1) = 20, a(2) = 30, … */
seq_t a(idx_t n)
{
    /* n is >= 1; adjust for zero‑based calculations */
    idx_t k = (n - 1) / 8;          /* block number (0‑based) */
    idx_t r = (n - 1) % 8;          /* position inside the 100‑number block */

    /* Compute the term in the 64‑bit integer type used by the build.  */
    return (seq_t)100 * k + (seq_t)10 * (r + 2);
}
