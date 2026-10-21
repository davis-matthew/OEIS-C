/*
 * A216115  The hyper-Wiener index of a link of n fullerenes C_{20} (see the
 *          Ghorbani and Hosseinzadeh reference).
 * https://oeis.org/A216115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n*(15n^3 +70n^2 + 134n - 117)
 * Range:     u8 none, u16 n <= 3, u32 n <= 71, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, but return 0 for n=0 */
    seq_t sn = (seq_t)n;           /* convert index to seq_t */
    seq_t n_sq = sn * sn;          /* n² */
    seq_t n_cub = n_sq * sn;       /* n³ */
    seq_t term = (seq_t)15 * n_cub + (seq_t)70 * n_sq + (seq_t)134 * sn - (seq_t)117;
    return sn * term * (seq_t)10; /* 10 * n * term */
}
