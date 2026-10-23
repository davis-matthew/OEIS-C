/*
 * A194757  Number of k such that {-k*Pi} > {-n*Pi}, where { } = fractional
 *          part.
 * https://oeis.org/A194757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (floor((n-1)/7)+1) * ((n-1) mod 7)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety, handle the (unused) index 0; the sequence is defined for n ≥ 1. */
    if (n == 0)
        return 0;

    idx_t m = n - 1;          /* m = n – 1 */
    idx_t q = m / 7;          /* q = ⌊(n‑1)/7⌋ */
    idx_t r = m % 7;          /* r = (n‑1) mod 7, 0 ≤ r ≤ 6 */
    idx_t res = (q + 1) * r;  /* (q+1)·r fits in idx_t for all 0 ≤ n < 2^64 */

    return (seq_t)res;
}
