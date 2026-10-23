/*
 * A023543  Convolution of natural numbers with A023533.
 * https://oeis.org/A023543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{i=1}^{d-1} i·L(i) + d·(offset+1) where d – the
 *            smallest integer with cum(d) ≥ n-1 offset= n - n_start(d)
 *            n_start(1)=2 , n_start(d)=d(d+1)(d+2)/6 for d≥2 L(1)=2 ,
 *            L(i)=(i+1)(i+2)/2 for i≥2 cum(d)= (d+1)(d+2)(d+3)/6 – 2
 * Range:     u8 n <= 62, u16 n <= 3433, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used, offset ≥ 1 */
    if (n == 1) return 1;
    /* find d such that cum(d) ≥ n-1 */
    idx_t d = 1;
    idx_t target = n - 1;
    for (;; ++d) {
        /* cum = (d+1)(d+2)(d+3)/6 – 2 */
        idx_t cum = ((d + 1) * (d + 2) * (d + 3) / 6) - 2;
        if (target <= cum) break;
    }
    /* compute start position of block d */
    seq_t n_start = 2;
    if (d > 1) n_start = (seq_t)d * (d + 1) * (d + 2) / 6;
    /* offset within the block */
    seq_t offset = (seq_t)n - n_start;
    /* prefix sum of i·L(i) for i = 1 .. d-1 */
    seq_t pref = 0;
    for (idx_t i = 1; i < d; ++i) {
        seq_t L = (i == 1) ? 2 : (seq_t)(i + 1) * (i + 2) / 2;
        pref += (seq_t)i * L;
    }
    /* compute a(n) */
    return 1 + pref + (seq_t)d * (offset + 1);
}
