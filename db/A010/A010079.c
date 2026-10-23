/*
 * A010079  Coordination sequence for net formed by holes in D_4 lattice.
 * https://oeis.org/A010079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(-4*(-1)^n+(3+(-1)^n)*n+6*n^3) for n>1 a(0) = 1, a(1) =
 *            16
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 710, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the two special cases first */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;

    /* convert index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* compute n^2 and n^3 */
    seq_t n2 = nn * nn;        /* n^2 */
    seq_t n3 = n2 * nn;        /* n^3 */

    /* 12 * n^3 term */
    seq_t part3 = n3 * (seq_t)12;

    /* 8n for even n, 4n for odd n */
    seq_t part2 = (n & (seq_t)1ULL) ? (nn * (seq_t)4) : (nn * (seq_t)8);

    /* base result */
    seq_t result = part3 + part2;

    /* adjust by +-8 depending on parity */
    if ((n & (seq_t)1ULL) == 0) {
        /* even n: subtract 8 */
        result -= (seq_t)8;
    } else {
        /* odd n: add 8 */
        result += (seq_t)8;
    }

    return result;
}
