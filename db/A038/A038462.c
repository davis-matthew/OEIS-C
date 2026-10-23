/*
 * A038462  Sums of 11 distinct powers of 2.
 * https://oeis.org/A038462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth smallest integer having exactly 11 bits set in
 *            its binary representation
 * Range:     u8 none, u16 n <= 4368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of 1‑bits in a value of type seq_t. */
static inline int popcount_seq(seq_t x)
{
    int c = 0;
    while (x)
    {
        x &= (x - 1); /* delete the lowest set bit */
        c++;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* a(1) = 2047 = (1 << 11) - 1, the smallest number with 11 bits set. */
    if (n == 0)
        return 0; /* undefined offset, but return 0 for safety */

    seq_t val = ((seq_t)1 << 11) - 1; /* 2047 */
    if (n == 1)
        return val;

    for (idx_t i = 1; i < n; ++i)
    {
        do
        {
            val++;                    /* try next integer */
        } while (popcount_seq(val) != 11);
    }
    return val;
}
