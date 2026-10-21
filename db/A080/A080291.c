/*
 * A080291  a(n) = (-1)^(n+1) * n^n * (n + (1/12) * (n^2-1)).
 * https://oeis.org/A080291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^(n+1) * n^n * (n + (1/12)(n^2-1))
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 14, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        if (exp > 1) base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety: offset is 1 */

    seq_t pow_n = pow_seq_t((seq_t)n, n);

    /* part1 = n^n * n */
    seq_t part1 = pow_n * (seq_t)n;

    /* part2 = (n^n * (n^2-1)) / 12 */
    seq_t n2     = (seq_t)n * (seq_t)n;
    seq_t part2  = pow_n * (n2 - 1);
    part2 /= (seq_t)12;

    seq_t result = part1 + part2;

    /* Apply the sign: negative when n is even, positive when n is odd */
    if ((n & 1) == 0) result = -result;

    return result;
}
