/*
 * A059261  Hilbert's Hamiltonian walk on N X N projected onto the first
 *          diagonal: M(3) (sum of the sequences A059252 and A059253).
 * https://oeis.org/A059261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = x(n) + y(n) where (x(n), y(n)) is the coordinates of *
 *            index n in the Hilbert curve of order ⌈log2 √(n+1)⌉
 * Range:     u8 n <= 9708, u16 n <= 9708, u32 n <= 9708, u64 n <= 9708,
 *            u128 n <= 9708, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute Hilbert curve coordinates (0‑based) for a given index n
   and return the sum of coordinates. */
seq_t a(idx_t n)
{
    /* Handle trivial case. */
    if (n == 0)
        return 0;

    /* Determine the order p such that 2^(2p) > n.  This is
     * equivalent to finding the smallest p with 2*p > ⌊log2 n⌋.
     * We avoid overflow by computing the bit‑length of n. */
    idx_t bitlen = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp >>= 1;
        bitlen++;
    }
    idx_t p = (bitlen + 1) / 2;          /* ceil(bitlen/2) */
    idx_t dim = (idx_t)1 << p;           /* dimension = 2^p */

    /* Hilbert index to (x, y) conversion */
    idx_t t   = n;
    idx_t x   = 0;
    idx_t y   = 0;
    for (idx_t s = 1; s < dim; s <<= 1) {
        idx_t rx = (t >> 1) & 1;          /* rx = (t/2) mod 2 */
        idx_t ry = (t ^ rx) & 1;          /* ry = (t XOR rx) mod 2 */

        if (ry == 0) {
            if (rx == 1) {
                x = (s - 1) - x;
                y = (s - 1) - y;
            }
            /* swap x and y */
            idx_t tmpxy = x;
            x = y;
            y = tmpxy;
        }

        x += s * rx;
        y += s * ry;
        t >>= 2;   /* advance to next pair of bits */
    }

    return (seq_t)(x + y);
}
