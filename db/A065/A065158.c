/*
 * A065158  Table of reduced binary string substitutions: a(i,j) is obtained
 *          by substituting i for each 1-bit in j, then dividing by i.
 * https://oeis.org/A065158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) = a(i,j) where (i,j) are obtained by numbering the grid
 *            with decreasing i along each diagonal: find d with (d-1)d/2 ≥ n,
 *            let offset = n - (d-2)(d-1)/2 - 1, then i = d-1-offset, j =
 *            1+offset. a(i,j) = (S(i,j))/i, where S(i,j) is built by
 *            replacing each 1-bit of j by the binary expansion of i...
 * Range:     u8 n <= 51, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: compute number of bits needed to represent x (x>0). */
static unsigned int bitlen_idx(idx_t x)
{
    unsigned int len = 0;
    while (x) {
        len++;
        x >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* --- Find the diagonal d such that (d-1)d/2 ≥ n ------------------- */
    idx_t d = 2;
    while (((__int128)(d - 1) * d / 2) < n) d++;

    /* Count of pairs before this diagonal */
    idx_t prev = ((d - 2) * (d - 1)) / 2;
    idx_t offset = n - prev - 1;   /* zero‑based position inside diagonal */

    /* Coordinates (i,j) of the pair */
    idx_t i = d - 1 - offset;
    idx_t j = 1 + offset;

    /* --- Construct S(i,j) ------------------------------------------- */
    seq_t i_seq = (seq_t)i;
    unsigned int len_i = bitlen_idx(i);

    seq_t R = 0;
    unsigned int len_j = bitlen_idx(j);
    for (int b = (int)len_j - 1; b >= 0; b--) {
        if ((j >> b) & ((idx_t)1)) {          /* bit is 1: append binary of i */
            R = (R << len_i) | i_seq;
        } else {                              /* bit is 0: append a single 0 */
            R <<= 1;
        }
    }

    /* --- Final result ------------------------------------------------- */
    return R / i_seq;
}
