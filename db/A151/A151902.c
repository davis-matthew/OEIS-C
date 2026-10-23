/*
 * A151902  a(n) = wt(k) + f(j) if n = 6k+j, 0 <= j < 6, where wt() =
 *          A000120(), f() = A151899().
 * https://oeis.org/A151902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = wt(k) + f(j) where n = 6*k + j, 0 <= j < 6, wt(k) is the
 *            binary weight (popcount) of k, and f is the constant array f(j)
 *            = {0,0,1,1,1,2}
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* split n into quotient k and remainder j */
    idx_t k = n / 6;
    unsigned int j = (unsigned int)(n % 6);

    /* binary weight (popcount) of k */
    seq_t pop = 0;
    idx_t t = k;
    while (t) {
        pop++;
        t &= t - 1;      /* clear lowest set bit */
    }

    /* constant correction terms for each residue class modulo 6 */
    static const seq_t f[6] = {0, 0, 1, 1, 1, 2};

    return pop + f[j];
}
