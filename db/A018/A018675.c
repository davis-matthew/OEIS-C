/*
 * A018675  Divisors of 837.
 * https://oeis.org/A018675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 3^(n-1) for 1 ≤ n ≤ 4, a(n) = 31·3^(n-5) for 5 ≤ n ≤ 8
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1 || n > 8)
        return 0;
    seq_t r = 1;
    if (n <= 4) {
        for (idx_t i = 0; i < n - 1; ++i)
            r *= (seq_t)3;
    } else {
        for (idx_t i = 0; i < n - 5; ++i)
            r *= (seq_t)3;
        r *= (seq_t)31;
    }
    return r;
}
