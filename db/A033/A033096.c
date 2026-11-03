/*
 * A033096  Number of 1's when k is written in base b for all b and k
 *          satisfying 2<=b<=n+1, 1<=k<=n.
 * https://oeis.org/A033096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{k=1}^n ∑_{b=2}^{k+1} (number of 1's in the base‑b
 *            representation of k)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: sum of 1's in the base‑b representation of all integers from 1 to m (inclusive) */
static seq_t sum_ones_in_base_upto(idx_t base, idx_t m)
{
    if (m == 0) return 0;
    seq_t total = 0;
    seq_t pow    = 1;
    while (pow <= m)
    {
        seq_t next = pow * (seq_t)base;
        seq_t high = (seq_t)(m / next);
        seq_t curr = (seq_t)((m / pow) % base);
        seq_t low  = (seq_t)(m % pow);

        seq_t add;
        if (curr == 1)
            add = high * pow + low + 1;
        else if (curr > 1)
            add = (high + 1) * pow;
        else
            add = high * pow;

        total += add;
        pow = next;
    }
    return total;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t b = 2; b <= n + 1; ++b)
    {
        seq_t upto_n   = sum_ones_in_base_upto(b, n);
        seq_t upto_low = 0;
        if (b >= 3)
            upto_low = sum_ones_in_base_upto(b, b - 2);
        total += (upto_n - upto_low);
    }
    return total;
}
