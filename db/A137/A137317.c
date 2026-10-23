/*
 * A137317  Array T(n,m) = 1 + (n+m-2) mod 26, 1 <= n,m <= 26, read by rows or
 *          columns.
 * https://oeis.org/A137317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, less, fini, full, word
 * Formula:   a(n) = 1 + ((n-1 + floor((n-1)/26)) mod 26)
 * Range:     u8 n <= 676, u16 n <= 676, u32 n <= 676, u64 n <= 676,
 *            u128 n <= 676, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n - 1;        /* zero‑based index of the term */
    idx_t q = k / 26;       /* floor((n-1)/26) */
    idx_t t = (k + q) % 26; /* (n-1 + floor((n-1)/26)) mod 26 */
    return (seq_t)(t + 1);  /* 1 ≤ a(n) ≤ 26 */
}
