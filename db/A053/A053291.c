/*
 * A053291  Nonsingular n X n matrices over GF(4).
 * https://oeis.org/A053291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{k=0}^{n-1} (4^n - 4^k)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (non‑singular n×n matrices over GF(4)) */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Compute 4^n */
    seq_t q = 1;
    for (idx_t i = 0; i < n; ++i)
        q *= (seq_t)4;

    /* Compute the product ∏_{k=0}^{n-1} (q - 4^k) */
    seq_t powk = 1;        /* 4^k */
    seq_t res  = 1;        /* running product */

    for (idx_t k = 0; k < n; ++k) {
        res *= (q - powk); /* q - powk is always >= 0 */
        powk *= (seq_t)4;  /* next power of 4 */
    }
    return res;
}
