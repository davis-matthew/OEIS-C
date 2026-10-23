/*
 * A037854  Sum_{i=1..m, d(i)>d(i-1)} d(i)-d(i-1), where Sum_{i=0..m} d(i)*4^i
 *          is the base 4 representation of n.
 * https://oeis.org/A037854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{i=1..m, d(i)>d(i-1)} (d(i)-d(i-1)) where n =
 *            sum_{k=0..m} d(k)*4^k (d(k) are the base‑4 digits of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1; a(0) is undefined */
        return 0;

    seq_t sum = 0;          /* result */
    idx_t prev = n % 4;     /* lowest base‑4 digit */
    n /= 4;                 /* shift to next digit */

    while (n > 0) {
        idx_t curr = n % 4; /* next digit */
        n /= 4;

        if (curr > prev)
            sum += (seq_t)(curr - prev);

        prev = curr;
    }

    return sum;
}
