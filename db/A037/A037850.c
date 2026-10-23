/*
 * A037850  a(n)=Sum{d(i-1)-d(i): d(i)<d(i-1), i=1,...,m}, where Sum{d(i)*9^i:
 *          i=0,1,...,m} is the base 9 representation of n.
 * https://oeis.org/A037850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=1..m} (d_{i-1} - d_i) for all i where d_i < d_{i-1},
 *            where the base‑9 representation of n is n = Σ_{i=0..m} d_i * 9^i
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;

    if (n == 0)
        return sum;          /* defined only for n ≥ 1, but safe for 0 */

    idx_t temp = n;
    idx_t prev_digit = temp % 9;
    temp /= 9;

    while (temp > 0) {
        idx_t curr_digit = temp % 9;
        if (curr_digit < prev_digit)
            sum += (seq_t)prev_digit - (seq_t)curr_digit;
        prev_digit = curr_digit;
        temp /= 9;
    }

    return sum;
}
