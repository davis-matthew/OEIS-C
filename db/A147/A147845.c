/*
 * A147845  Odd positive integers a(n) such that for every odd integer m>=7
 *          there exists a unique representation of the form
 *          m=a(p)+2a(q)+4a(r).
 * https://oeis.org/A147845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A033045(n-1)+1 where A033045(m) = interpret binary(m)
 *            as base‑8
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) using the observation that A033045(m) equals the sum of
   2^(3*i) for every bit i set in m. */
seq_t a(idx_t n)
{
    idx_t m = n - 1;           /* index for A033045 */
    seq_t val = 0;             /* value of A033045(m) */
    seq_t shift = 1;           /* 2^(3*i) for current bit */

    while (m)
    {
        if (m & 1)          /* if the least significant bit is 1 add the 2^(3*i) */
            val += shift;
        shift <<= 3;        /* next power of 8 (i.e., 2^(3*(i+1))) */
        m >>= 1;            /* move to next bit of m */
    }

    return val * 2 + 1;      /* a(n) = 2*A033045(m)+1 */
}
