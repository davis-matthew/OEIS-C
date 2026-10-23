/*
 * A043531  Number of distinct base-4 digits of n.
 * https://oeis.org/A043531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct digits in the base‑4 representation of
 *            n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long t = (unsigned long long)n;  /* copy of idx_t */
    unsigned int mask = 0;                         /* bitmask for digits 0‑3 */

    while (t)
    {
        /* extract the low 2 bits (a base‑4 digit) and set corresponding bit */
        mask |= 1u << (t & 3u);
        /* shift by 2 bits to process next digit */
        t >>= 2;
    }

    /* count the number of bits set in mask (i.e., distinct digits used) */
    int count = 0;
    unsigned int m = mask;
    while (m)
    {
        count += m & 1u;
        m >>= 1;
    }

    return (seq_t)count;
}
