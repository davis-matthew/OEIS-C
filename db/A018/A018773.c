/*
 * A018773  Divisors of 1010.
 * https://oeis.org/A018773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n‑th divisor of 1010 (in increasing order)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1010 = 2 * 5 * 101. The divisors are all products of
       2^i * 5^j * 101^k with i,j,k in {0,1}.  The order produced
       by iterating i, then j, then k gives the OEIS ordering. */
    if (n < 1 || n > 8)
        return 0;          /* outside the range of defined divisors */

    /* Map (n-1) to exponents i (for 2), j (for 5), k (for 101). */
    idx_t m = n - 1;
    unsigned i = (m >> 0) & 1U;
    unsigned j = (m >> 1) & 1U;
    unsigned k = (m >> 2) & 1U;

    seq_t val = 1;
    if (i)
        val *= (seq_t)2;
    if (j)
        val *= (seq_t)5;
    if (k)
        val *= (seq_t)101;
    return val;
}
