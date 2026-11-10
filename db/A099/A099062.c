/*
 * A099062  A bisection of A000960.
 * https://oeis.org/A099062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000960(2*n+2) (bisection of A000960); A000960(m): start
 *            with m^2, then successively round down (strictly below) to a
 *            multiple of m-1, m-2, ..., 1
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 45 (2026-11-07)
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
    return A000960_a(2 * n + 2);
}
