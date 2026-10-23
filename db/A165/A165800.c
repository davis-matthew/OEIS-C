/*
 * A165800  Powers of 50.
 * https://oeis.org/A165800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 50^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth power of 50 using only integral operations that work for
   both fixed‑width unsigned types (uint8_t … uint128_t) and arbitrary‑precision
   integer classes.  The function is safe for any n ≥ 0. */
seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;              /* a(0) = 1 */
    for (idx_t i = 0; i < n; ++i) {
        res *= (seq_t)50;              /* multiply by 50 each step */
    }
    return res;
}
