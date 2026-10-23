/*
 * A047525  Numbers that are congruent to {0, 2, 7} mod 8.
 * https://oeis.org/A047525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th non‑negative integer congruent to 0, 2 or 7
 *            modulo 8. Closed form: if n % 3 == 0 : a(n) = 8*(n/3) - 1 if n %
 *            3 == 1 : a(n) = 8*((n+2)/3) - 8 if n % 3 == 2 : a(n) =
 *            8*((n+1)/3) - 6 (all arithmetic performed in seq_t) The function
 *            works for any call order and O(1) time
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    seq_t sn = (seq_t)n;
    seq_t ans;
    if (r == 0) {
        seq_t k = sn / 3;
        ans = 8 * k - 1;
    } else if (r == 1) {
        seq_t k = (sn + 2) / 3;
        ans = 8 * k - 8;
    } else { /* r == 2 */
        seq_t k = (sn + 1) / 3;
        ans = 8 * k - 6;
    }
    return ans;
}
