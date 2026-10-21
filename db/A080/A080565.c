/*
 * A080565  Binary expansion of n has form 11**...*1.
 * https://oeis.org/A080565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2^floor(log2(4*(n-1))) + 2*n - 1  for n > 1,
 *            a(1) = 3
 *
 * Sequence A080565: Binary expansion of n has form 11***1.
 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;          /* a(1) = 3 */

    /* tmp = 4*(n-1)  using unsigned arithmetic */
    seq_t tmp = (((seq_t)n) - 1) << 2;     /* 4*(n-1) */

    /* compute k = floor(log2(tmp)) – highest set bit index */
    idx_t k = 0;
    seq_t t = tmp;
    while (t >>= 1) k++;                  /* shift until zero, count shifts */

    seq_t pow2 = ((seq_t)1) << k;         /* 2^k  (k <= 63 for 64‑bit seq_t) */

    return pow2 + ((seq_t)n << 1) - 1;    /* pow2 + 2*n - 1 */
}
