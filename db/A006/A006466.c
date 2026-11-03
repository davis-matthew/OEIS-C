/*
 * A006466  Continued fraction expansion of C = 2*Sum_{n>=0} 1/2^(2^n).
 * https://oeis.org/A006466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(5n) = a(5n+1) = a(2) = a(5n+3) = a(20n+14) = a(40n+9) = 1,
 *            a(20n+4) = a(40n+29) = 2, a(5n+2) = 3 - a(5n-1) for n>=1,
 *            a(20n+19) = a(10n+9)
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9487 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n % 40 == 29) {
        return 2;
    }
    if (n % 40 == 9) {
        return 1;
    }
    if (n % 20 == 19) {
        idx_t m = (n - 19) / 20;
        return a(10 * m + 9);
    }
    if (n % 20 == 14) {
        return 1;
    }
    if (n % 20 == 4) {
        return 2;
    }
    if (n % 5 == 2) {
        if (n == 2) {
            return 1;
        }
        return 3 - a(n - 3);
    }
    if (n % 5 == 3) {
        return 1;
    }
    if (n % 5 == 1) {
        return 1;
    }
    return 1;
}
