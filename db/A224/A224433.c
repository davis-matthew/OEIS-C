/*
 * A224433  The Wiener index of the dendrimer D_3[n], defined pictorially in
 *          the A. R. Ashrafi et al. reference.
 * https://oeis.org/A224433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -12882 + 44604*2^n - 31548*4^n + 21780*n*4^n - 7260*n*2^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 7, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n and 4^n using only seq_t operations */
    seq_t pow2 = 1;
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;
        pow4 *= 4;
    }

    /* terms of the closed‑form expression */
    seq_t t1 = 44604 * pow2;
    seq_t t2 = 21780 * (seq_t)n * pow4;
    seq_t t3 = 31548 * pow4;
    seq_t t4 = 7260 * (seq_t)n * pow2;

    /* combine with unsigned arithmetic – intermediate results are
       guaranteed non‑negative for all n>=0, so subtraction is safe. */
    seq_t result = t1 + t2;
    result -= t3;
    result -= t4;
    result -= 12882;

    return result;
}
