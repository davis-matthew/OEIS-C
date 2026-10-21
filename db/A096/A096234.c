/*
 * A096234  Base 10 "digit addition generators": a(n) = smallest m such that m
 *          + (sum of digits of m) = n, or -1 if no such m exists.
 * https://oeis.org/A096234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = smallest m such that m + digit_sum(m) = n, or -1 if none
 *            exist. The implementation searches only the limited range
 *            [n-9*digits(n), n] because the digit sum of any m ≤ n is at most
 *            9 times its number of decimal digits
 * Range:     i8 n <= 132, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t digit_sum_seq_t(seq_t v)
{
    seq_t s = 0;
    while (v > 0)
    {
        s += v % 10;
        v /= 10;
    }
    return s;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;                     // target value
    idx_t  digits = 0;
    idx_t  t = n;

    // count number of decimal digits in n (handle n==0)
    while (t > 0)
    {
        digits++;
        t /= 10;
    }
    if (digits == 0)
        digits = 1;                         // n == 0

    idx_t max_digit_sum = 9 * digits;
    idx_t lower;
    if (n >= max_digit_sum)
        lower = n - max_digit_sum;
    else
        lower = 0;                          // avoid underflow for small n

    for (idx_t m = lower; m <= n; ++m)
    {
        seq_t M = (seq_t)m;
        seq_t s = digit_sum_seq_t(M);
        if (M + s == N)
            return M;                       // smallest found
    }
    return -1;                               // no generator
}
