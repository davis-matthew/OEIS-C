/*
 * A006993  n! in base n.
 * https://oeis.org/A006993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = n! written in base n, with its base-n digits read as a
 *            decimal number (a(1) = 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;

    seq_t factorial = 1;
    for (idx_t i = 2; i <= n; i++) {
        factorial *= (seq_t)i;
    }

    seq_t base = (seq_t)n;
    seq_t temp = factorial;
    seq_t result = 0;
    seq_t place = 1;
    while (temp > 0) {
        seq_t digit = temp % base;
        temp /= base;
        result += digit * place;
        if (temp > 0) place *= 10;
    }
    return result;
}
