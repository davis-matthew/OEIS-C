/*
 * A046192  Octagonal hexagonal numbers.
 * https://oeis.org/A046192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9602*a(n-1) - a(n-2) + 2200 initial values: a(1)=1,
 *            a(2)=11781
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* offset is 1 */

    if (n == 1)
        return (seq_t)1;

    if (n == 2)
        return (seq_t)11781;

    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)11781;
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)9602 * prev1 - prev2 + (seq_t)2200;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
