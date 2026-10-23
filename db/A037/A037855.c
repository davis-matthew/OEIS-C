/*
 * A037855  Sum{d(i)-d(i-1): d(i)>d(i-1), i=1,...,m}, where Sum{d(i)*5^i:
 *          i=0,1,...,m} is base 5 representation of n.
 * https://oeis.org/A037855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=1}^{m} max(0, d_i - d_{i-1}), where the digits d_i
 *            (d_0 the least significant) of n are taken in base 5 (n = Σ d_i
 *            * 5^i)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1, but 0 is harmless */
        return 0;

    seq_t   sum = 0;      /* result accumulator */
    idx_t   prev = n % 5; /* least‑significant digit */
    n /= 5;

    while (n > 0) {
        idx_t cur = n % 5;          /* next digit */
        if (cur > prev)
            sum += (seq_t)(cur - prev);
        prev = cur;
        n /= 5;
    }
    return sum;
}
