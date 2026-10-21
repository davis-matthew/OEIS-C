/*
 * A088864  Maximum of the products of left and right parts when splitting the
 *          binary representation of n.
 * https://oeis.org/A088864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Max{ floor(n/(2^k)) * (n mod 2^k) } for k≥1
 * Range:     u8 n <= 302, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (indices start at 1; a(1)=0, a(2)=0, etc.) */
seq_t a(idx_t n)
{
    /* trivial small cases */
    if (n <= 1)
        return 0;

    /* compute bit length of n (number of bits needed to represent n) */
    idx_t bits = 0;
    idx_t tmp  = n;
    while (tmp)
    {
        ++bits;
        tmp >>= 1;
    }

    seq_t max_prod = 0;          /* maximum product seen so far   */
    seq_t seq_n   = (seq_t)n;    /* copy of n as seq_t if needed */

    for (idx_t k = 1; k < bits; ++k)
    {
        /* low part: n & (2^k – 1)                                  */
        idx_t mask  = ((idx_t)1 << k) - 1;
        seq_t left  = (seq_t)(n >> k);    /* high part  */
        seq_t right = (seq_t)(n & mask);  /* low part   */
        seq_t prod  = left * right;       /* product    */

        if (prod > max_prod)
            max_prod = prod;
    }

    return max_prod;
}
