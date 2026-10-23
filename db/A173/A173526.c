/*
 * A173526  a(n) = 1 + A053827(n-1), where A053827 is the sum-of-digits
 *          function in base 6.
 * https://oeis.org/A173526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + A053827(n-1) where A053827 is the sum of the base‑6
 *            digits of (n-1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t x = n - 1;        /* n >= 1, so n-1 is non‑negative */
    seq_t sum = 0;
    while (x != 0) {
        sum += (seq_t)(x % 6);   /* add the least significant base‑6 digit */
        x /= 6;                  /* shift right one digit */
    }
    return sum + 1;              /* add the +1 offset */
}
