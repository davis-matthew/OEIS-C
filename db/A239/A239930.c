/*
 * A239930  Number of distinct quarter-squares dividing n.
 * https://oeis.org/A239930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct quarter‑squares that divide n
 * Range:     u8 n <= 2183, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k)
    {
        idx_t m = k / 2;                 /* floor(k/2) */
        seq_t q;
        if ((k & 1) == 0)                /* k even: q = (k/2)^2 */
            q = (seq_t)m * (seq_t)m;
        else                              /* k odd : q = (k/2)^2 + (k/2) */
            q = (seq_t)m * (seq_t)m + (seq_t)m;

        if (q == 0)                      /* skip the duplicate 0 (k = 1) */
            continue;
        if (q > (seq_t)n)                /* further k will only increase q */
            break;
        if (n % (idx_t)q == 0)           /* q divides n */
            ++count;
    }
    return (seq_t)count;
}
