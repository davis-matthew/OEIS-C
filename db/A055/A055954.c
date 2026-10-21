/*
 * A055954  a(n) = n + reversal of base 7 digits of n (written in base 10).
 * https://oeis.org/A055954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n + reverse_base7(n) where reverse_base7(n) is the
 *            reversal of the base‑7 digits of n, interpreted again as a
 *            base‑10 number
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t rev = 0;          /* value of reversed base‑7 digits */
    idx_t t = n;            /* work on a copy to avoid modifying n */

    while (t > 0) {
        rev = rev * 7 + (t % 7);  /* add least significant base‑7 digit */
        t /= 7;                  /* shift right one base‑7 digit */
    }

    return (seq_t)n + rev;      /* final result */
}
