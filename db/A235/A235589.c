/*
 * A235589  The periodic part of the decimal expansion of m/(m+1), for those
 *          m/(m+1) that have pure periods.
 * https://oeis.org/A235589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 5, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the periodic part of the decimal expansion of
   m/(m+1) for the nth integer m with gcd(m+1,10)=1.
   The nth such m is given by
       m = 10*floor((n-1)/4) + {2,6,8,10}[(n-1)%4]   */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, safe guard */

    /* Determine the nth integer m with gcd(m+1,10)=1 */
    static const idx_t offset[4] = {2, 6, 8, 10};

    idx_t block = (n - 1) / 4;
    idx_t rem   = (n - 1) % 4;

    seq_t m  = (seq_t)block * 10 + (seq_t)offset[rem];
    seq_t denom = m + (seq_t)1;

    /* Generate the decimal digits of m/(m+1) and build the
       integer value of the repeating block. */
    seq_t r = m;
    seq_t result = 0;

    do
    {
        seq_t t = r * (seq_t)10;
        seq_t digit = t / denom;
        result = result * (seq_t)10 + digit;
        r = t % denom;
    }
    while (r != m);

    return result;
}
