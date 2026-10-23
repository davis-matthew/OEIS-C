/*
 * A021605  Decimal expansion of 1/601.
 * https://oeis.org/A021605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^(n+1))/601) mod 10, computed by standard
 *            long‑division digit extraction
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 1;       /* current remainder (initially 1/601)           */
    seq_t digit = 0;   /* digit to be returned                        */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t temp = r * 10;   /* shift remainder by one decimal place   */
        digit = temp / 601;    /* integer division gives the next digit   */
        r = temp % 601;        /* new remainder for next iteration       */
    }
    return digit;
}
