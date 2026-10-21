/*
 * A076839  A simple example of the Lyness 5-cycle: a(1) = a(2) = 1; a(n) =
 *          (a(n-1)+1)/a(n-2) (for n>2).
 * https://oeis.org/A076839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(1) = 1, a(2) = 1; for n > 2, a(n) = (a(n-1)+1)/a(n-2); The
 *            sequence is periodic with period 5: 1,1,2,3,2 repeating
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[5] = {(seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)2};
    /* Use 1‑based indexing: (n-1) % 5 gives 0…4 for n=1…∞ */
    idx_t r = (n - 1) % 5;
    return pattern[r];
}
