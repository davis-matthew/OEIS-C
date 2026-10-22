/*
 * A007471  Sum of digits of n a(n) is n ( = A003634/n), or 0 if no such
 *          number exists.
 * https://oeis.org/A007471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest m with digitsum(n*m) = m, or 0 if none (search).
 *            m == n*m (mod 9), so 9/gcd(n-1,9) divides m, and m <=
 *            9*(digits(n)+digits(m))
 * Range:     u8 n <= 20, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t digit_sum(seq_t x) {
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

static idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    idx_t g = gcd_idx(n - 1, 9);
    idx_t step = 9 / g;
    idx_t dn = 0, t = n;
    while (t > 0) { dn++; t /= 10; }
    idx_t limit = 9 * (dn + 4);
    for (idx_t m = step; m <= limit; m += step) {
        if (digit_sum(sn * (seq_t)m) == (seq_t)m) {
            return (seq_t)m;
        }
    }
    return 0;
}
