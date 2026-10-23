/*
 * A037904  Greatest digit of n - least digit of n.
 * https://oeis.org/A037904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = A054055(n)-A054054(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* guard for safety, offset actually starts at 1 */
    idx_t tmp = n;
    int maxd = 0;
    int mind = 9;
    while (tmp > 0)
    {
        int d = (int)(tmp % 10);
        if (d > maxd) maxd = d;
        if (d < mind) mind = d;
        tmp /= 10;
    }
    return (seq_t)(maxd - mind);
}
