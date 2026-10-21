/*
 * A059561  Beatty sequence for log(Pi).
 * https://oeis.org/A059561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*log(pi))
 * Range:     u8 n <= 223, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  We use a fixed‐precision integer approximation of log(pi) with 12 digits
 *  after the decimal point: SCALE = 1,000,000,000,000.  L = floor(log(pi)*SCALE)
 *  with L = 1,144,729,885,849.  The product n*L fits in 128 bits for n up
 *  to 10^4, and the division by SCALE yields the exact integer floor(n*log(pi)). */
seq_t a(idx_t n)
{
    /* L = floor(log(pi) * 1e12)  */
    const unsigned long long L    = 1144729885849ULL;
    const unsigned long long SCALE = 1000000000000ULL;

    /* Use unsigned __int128 for the intermediate product to avoid overflow. */
    unsigned __int128 prod = (unsigned __int128)n * L;
    return (seq_t)(prod / SCALE);
}
