/*
 * A206117  Number of (n+1) X 8 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two counterclockwise and two clockwise edge
 *          increases.
 * https://oeis.org/A206117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7075*a(n-3) for n>10; a(1..10) given
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 11, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* precomputed first ten terms */
    static const seq_t base[10] = {
        (seq_t)5538732ULL,
        (seq_t)16072760ULL,
        (seq_t)59892000ULL,
        (seq_t)1542266208ULL,
        (seq_t)18371031840ULL,
        (seq_t)373319837280ULL,
        (seq_t)10214794664928ULL,
        (seq_t)127417290645600ULL,
        (seq_t)2621909765268000ULL,
        (seq_t)72022987084456800ULL
    };

    if (n <= 10) {
        return base[n - 1];
    }

    idx_t m = n;
    seq_t mult = (seq_t)1;
    while (m > 10) {
        mult *= (seq_t)7075;
        m -= 3;
    }
    seq_t val = base[m - 1];
    return val * mult;
}
