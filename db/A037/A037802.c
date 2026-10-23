/*
 * A037802  Number of i such that d(i) < d(i-1), where Sum_{i=0..m} d(i)*4^i
 *          is the base-4 representation of n.
 * https://oeis.org/A037802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i (≥1) such that the base‑4 digit d(i)
 *            of n is less than the next less significant digit d(i-1). Here n
 *            = Σ d(i)*4^i
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;

    idx_t x = n;
    seq_t count = 0;
    uint8_t prev = x % 4u;
    x /= 4u;

    while (x > 0u) {
        uint8_t cur = x % 4u;
        if (cur < prev)
            ++count;
        prev = cur;
        x /= 4u;
    }
    return count;
}
