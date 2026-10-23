/*
 * A028902  Map n = Sum c_i 10^i to a(n) = Sum c_i 7^i.
 * https://oeis.org/A028902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ digit_i(n)·7^i where n = Σ digit_i(n)·10^i (decimal
 *            representation)
 * Range:     u8 n <= 483, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by treating the decimal digits of n as coefficients of powers of 7. */
seq_t a(idx_t n) {
    idx_t tmp = n;
    seq_t result = 0;
    seq_t pow7   = 1;            /* 7^0 initially */

    while (tmp > 0) {
        uint64_t digit = tmp % 10;      /* extract least‐significant decimal digit */
        result += (seq_t)digit * pow7;  /* add digit·7^i to the result */
        tmp /= 10;                      /* shift to next digit */
        pow7 = pow7 * 7;                /* advance to next power of 7 */
    }
    return result;
}
