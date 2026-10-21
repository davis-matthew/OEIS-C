/*
 * A077353  a(n) = (concatenation in ascending order of divisors of 5^n)/5^n.
 * https://oeis.org/A077353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = (concatenation of divisors of 5^n)/5^n. Recurrence: a(0)
 *            = 1, a(n) = (2*10^(d(n)-1))*a(n-1) + 1, where d(n) is the number
 *            of decimal digits of 5^n. d(n) increases by at most 1 each step;
 *            we keep it in sync by comparing 5^n with the next power of 10
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = 1;          /* 5^k, starting with k = 0 */
    seq_t pow10_cur = 1;    /* 10^(digits(cur)-1), initially 10^0 = 1 */
    seq_t a_val = 1;        /* a(0) */
    idx_t digits = 1;       /* current number of digits of cur */

    /* The loop runs from 1 to n, updating the state each time. */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* 5^k becomes 5^(k+1) */
        cur = cur * (seq_t)5;

        /* Threshold for the next digit count: 10^digits */
        seq_t threshold = pow10_cur * (seq_t)10;

        /* If 5^(k+1) >= 10^digits, the digit count increases. */
        if (cur >= threshold)
        {
            pow10_cur = threshold;  /* now pow10_cur = 10^digits = 10^(digits+1-1) */
            ++digits;               /* new digit count */
        }

        /* factor = 2 * 10^(digits-1) == 2 * pow10_cur */
        seq_t factor = pow10_cur * (seq_t)2;

        /* a(n) = factor * a(n-1) + 1 */
        a_val = a_val * factor + (seq_t)1;
    }

    return a_val;
}
