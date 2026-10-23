/*
 * A141747  a(n) is the number of nonnegative integer pairs i,j such that n =
 *          2^i + 3^j.
 * https://oeis.org/A141747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (i,j) ≥ 0 such that n = 2^i + 3^j
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 0 (no representation with two positive powers) */
    if (n < 2)
        return (seq_t)0;

    seq_t count = 0;

    /* Pre‑compute powers of 3 up to n */
    idx_t pow3[64];
    idx_t p3cnt = 0;
    idx_t p = 1;
    while (p <= n) {
        pow3[p3cnt++] = p;
        if (p > n / 3) break;   /* avoid overflow of p *= 3 */
        p *= 3;
    }

    /* Iterate over powers of 2 up to n and test membership in pow3 */
    idx_t p2 = 1;
    while (p2 <= n) {
        idx_t rem = n - p2;     /* non‑negative because p2 ≤ n */

        /* Linear search – small array, fast */
        for (idx_t k = 0; k < p3cnt; ++k) {
            if (pow3[k] == rem) {
                ++count;
                break;
            }
        }

        if (p2 > n / 2) break;   /* avoid overflow of p2 *= 2 */
        p2 <<= 1;
    }

    return count;
}
