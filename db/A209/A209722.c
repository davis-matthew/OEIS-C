/*
 * A209722  1/4 the number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock
 *          having distinct clockwise edge differences.
 * https://oeis.org/A209722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^(n/2-1)+2 for even n; a(n)=2^((n+1)/2)+2 for odd n
 *            Recurrence: a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3), with
 *            a(1)=4,a(2)=5,a(3)=6
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power_of_two(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) {
        res *= 2;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n % 2 == 0) {
        idx_t half = n / 2;
        seq_t p = power_of_two(half - 1);
        return 3 * p + 2;
    } else {
        idx_t half = (n + 1) / 2;
        seq_t p = power_of_two(half);
        return p + 2;
    }
}
