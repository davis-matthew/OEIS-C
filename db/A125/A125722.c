/*
 * A125722  Greatest product of two numbers made up together of decimal digits
 *          0 to n once.
 * https://oeis.org/A125722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) is the maximum product of two numbers that together use *
 *            the digits 0,1,…,n exactly once. The greedy algorithm that
 *            always * appends the next largest unused digit to the currently
 *            smaller number * produces the optimal partition
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t X = 0;
    seq_t Y = 0;
    for (idx_t d = n; ; --d) {
        if (X <= Y)
            X = X * 10 + (seq_t)d;
        else
            Y = Y * 10 + (seq_t)d;
        if (d == 0) break;
    }
    return X * Y;
}
