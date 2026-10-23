/*
 * A127428  v_5( (10n)! ) - v_5( (5n)! ), where v_p(N) is the p-adic valuation
 *          defined in A112765.
 * https://oeis.org/A127428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = a([(n+2)/5]) + n, a(0) = 0 (equivalent to a(n) =
 *            sum_{k>=0} round(n/5^k))
 * Range:     u8 n <= 204, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    while (n > 0) {
        sum += (seq_t)n;
        idx_t next = n / 5;
        /* Add carry if (n + 2) exceeds a multiple of 5 */
        if ((n % 5) + 2 >= 5)
            next += 1;
        n = next;
    }
    return sum;
}
