/*
 * A234292  E.g.f. satisfies: A(x) = 1 + A(x)^4 * Integral 1/A(x) dx.
 * https://oeis.org/A234292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 11*a(n) = 3*(10*n-37)*a(n-1) + 64*(3*n-5)*(3*n-4)*a(n-2);
   a(0) = 1, a(1) = 1. */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 1;

    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 1;   /* a(1) */
    seq_t cur, term1, term2, sub, num;

    for (idx_t i = 2; i <= n; ++i) {
        /* term2 = 64*(3*i-5)*(3*i-4)*prev2 */
        term2 = 64 * (seq_t)(3*i - 5) * (seq_t)(3*i - 4) * prev2;

        if (i == 2 || i == 3) {
            /* 37-10*i is positive for i=2,3 */
            sub  = 3 * (seq_t)(37 - 10*i) * prev1;   /* positive value */
            num  = term2 - sub;
        } else {
            term1 = 3 * (seq_t)(10*i - 37) * prev1;  /* all factors positive */
            num   = term1 + term2;
        }

        cur = num / 11;          /* division is exact */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
#pragma GCC diagnostic pop
