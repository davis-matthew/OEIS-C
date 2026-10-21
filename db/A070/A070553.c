/*
 * A070553  Rectangular array read by rows: n-th row gives the 7 numbers
 *          k*10^n mod 7 for 0 <= k < 7.
 * https://oeis.org/A070553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = ((n-1) mod 7) * 10^((n-1)/7) mod 7 . The factor
 *            10^((n-1)/7) cycles with period 6: 10^0 = 1, 10^1 = 3, 10^2 = 2,
 *            10^3 = 6, 10^4 = 4, 10^5 = 5 (mod 7)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t pow10_mod7[6] = { 1, 3, 2, 6, 4, 5 };

seq_t a(idx_t n)
{
    /* n is >=1; compute zero‑based index */
    idx_t zero = n - 1;

    /* column (k) in the 7 numbers of the row */
    idx_t k = zero % 7;

    /* row number (r) */
    idx_t r = zero / 7;

    /* power of 10 modulo 7 for this row, period 6 */
    seq_t pow = pow10_mod7[r % 6];

    /* compute k * pow modulo 7 */
    return (seq_t)((k * pow) % 7);
}
