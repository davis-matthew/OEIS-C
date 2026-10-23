/*
 * A028151  Expansion of 1/((1-4x)(1-7x)(1-9x)(1-12x)).
 * https://oeis.org/A028151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12^(n+3) - 4*9^(n+3) + 4*7^(n+3) - 4^(n+3)) / 120 Using
 *            the linear recurrence derived from the generating function: a(0)
 *            = 1, a(1) = 32, a(2) = 657, a(3) = 11056 a(n) = 32*a(n-1) -
 *            367*a(n-2) + 1776*a(n-3) - 3024*a(n-4) (n >= 4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 32;
    if (n == 2) return 657;
    if (n == 3) return 11056;

    /* keep the last four terms, indexed from a(n-4) to a(n-1) */
    seq_t prev4 = 1;      /* a(n-4) */
    seq_t prev3 = 32;     /* a(n-3) */
    seq_t prev2 = 657;    /* a(n-2) */
    seq_t prev1 = 11056;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        /* recurrence: a(i) = 32*a(i-1) - 367*a(i-2) + 1776*a(i-3) - 3024*a(i-4) */
        cur = 32 * prev1
            - 367 * prev2
            + 1776 * prev3
            - 3024 * prev4;

        /* shift the sliding window */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
