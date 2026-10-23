/*
 * A124356  Number of (directed) Hamiltonian cycles on the Moebius ladder
 *          graph M_n (for n >= 4).
 * https://oeis.org/A124356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n if n is odd, 2*n+4 if n is even
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rn = (seq_t)n;            /* convert index to seq_t */
    seq_t res = rn << 1;            /* 2*n */

    if ((n & 1ULL) == 0)            /* even n */
        res += 4;

    return res;
}
