/*
 * A097769  Pell equation solutions (12*a(n))^2 - 145*b(n)^2 = -1 with
 *          b(n):=A097770(n), n >= 0.
 * https://oeis.org/A097769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 578*a(n-1) - a(n-2);
 * with a(0) = 1, a(1) = 579;
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)579;
    }

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)579; /* a(1) */
    seq_t curr   = prev1;     /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)578 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
