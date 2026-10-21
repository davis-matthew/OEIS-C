/*
 * A216869  The smallest non-constant arithmetic progression of integer
 *          squares of maximal length three.
 * https://oeis.org/A216869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, bref
 * Formula:   a(n) = (k^2, (5k)^2, (7k)^2) where k = floor((n-1)/3)+1, and n
 *            mod 3 selects which square to return: n≡1 → k^2, n≡2 → (5k)^2,
 *            n≡0 → (7k)^2
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* a(0) is undefined, return 0 */

    idx_t k = (n - 1) / 3 + 1;          /* k = block index + 1 */
    idx_t r = (n - 1) % 3;              /* position: 0,1,2 */

    seq_t kk = (seq_t)k * k;            /* k^2 */

    if (r == 0)          /* n ≡ 1 (mod 3) */
        return kk;

    /* compute (5k)^2 or (7k)^2 */
    seq_t k_mul;
    if (r == 1)
        k_mul = (seq_t)k * 5;          /* 5k */
    else
        k_mul = (seq_t)k * 7;          /* 7k */

    return k_mul * k_mul;              /* (5k)^2 or (7k)^2 */
}
