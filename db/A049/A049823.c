/*
 * A049823  a(n)=number of 2's in n-th row of array T given by A049816.
 * https://oeis.org/A049823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k (1 ≤ k ≤ n) such that the Euclidean algorithm
 *            on (n, k) has exactly 2 non‑zero remainders. This is equivalent
 *            to: if d1 = n % k > 0 and d2 = k % d1 > 0 then d1 % d2 == 0. The
 *            function evaluates this condition directly in O(n)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)            /* n = 1 gives no k with two remainders */
        return 0;

    seq_t cnt = 0;
    for (idx_t k = 1; k <= n - 1; ++k)
    {
        idx_t d1 = n % k;
        if (d1 == 0)
            continue;

        idx_t d2 = k % d1;
        if (d2 == 0)
            continue;

        if (d1 % d2 == 0)
            ++cnt;
    }
    return cnt;
}
