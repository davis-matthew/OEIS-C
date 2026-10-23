/*
 * A166727  Positive integers with English names ending in "r".
 * https://oeis.org/A166727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = 100*floor((n-1)/9) + 10*((n-1)%9==0 ? 0 : ((n-1)%9)+1) +
 *            4 * (OEIS A166727: Positive integers with English names ending
 *            in "r")
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* assume n >= 1; behaviour for n < 1 is unspecified */
    idx_t n_minus1 = n - 1;
    idx_t k = n_minus1 / 9;          /* number of full 9‑term blocks */
    idx_t r = n_minus1 % 9;          /* position within the current block (0..8) */
    idx_t offset = (r == 0) ? 0 : r + 1;  /* skip the missing “14” → 10·i + 4 */

    return (seq_t)100 * (seq_t)k + (seq_t)10 * (seq_t)offset + (seq_t)4;
}
