/*
 * A004695  a(n) = floor(Fibonacci(n)/2).
 * https://oeis.org/A004695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + [n mod 3 = 0], a(0) = a(1) = a(2) = 0
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 49, u64 n <= 95, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prev2 = 0; /* a(i-2) */
    seq_t prev1 = 0; /* a(i-1) */
    idx_t i;

    if (n <= 2)
        return 0;

    for (i = 3; i <= n; i++) {
        seq_t cur = prev1 + prev2;
        if (i % 3 == 0)
            cur += 1;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
