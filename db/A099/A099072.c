/*
 * A099072  First differences of A000960, divided by 2.
 * https://oeis.org/A099072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A000960(n+1) - A000960(n))/2, where A000960(m) starts
 *            with m^2 and rounds down (strictly below) to a multiple of m-1,
 *            m-2, ..., 1
 * Range:     u8 n <= 14, u16 n <= 538, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9260 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A000960_a(idx_t n) {
    seq_t result = (seq_t)n * (seq_t)n;

    for (seq_t divisor = (seq_t)n - 1; divisor > 0; divisor--) {
        seq_t quotient = result / divisor;
        if (result % divisor == 0) {
            quotient--;
        }
        result = quotient * divisor;
    }

    return result;
}

seq_t a(idx_t n) {
    seq_t hi = A000960_a(n + 1);
    seq_t lo = A000960_a(n);
    return (hi - lo) / 2;
}
