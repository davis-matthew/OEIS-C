/*
 * A037857  Sum{d(i)-d(i-1): d(i)>d(i-1), i=1,...,m}, where Sum{d(i)*7^i:
 *          i=0,1,...,m} is base 7 representation of n.
 * https://oeis.org/A037857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ max(0, d(i) – d(i-1)) for i = 1..m, where d(i) are the
 *            base‑7 digits of n (least significant digit has index 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t x = n;

    if (x == 0)
        return 0;

    /* least significant digit */
    seq_t prev = x % 7;
    x /= 7;

    while (x > 0) {
        seq_t cur = x % 7;
        if (cur > prev)
            result += (cur - prev);
        prev = cur;
        x /= 7;
    }
    return result;
}
