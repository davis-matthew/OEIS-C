/*
 * A230349  Number of permutations of [2n+9] in which the longest increasing
 *          run has length n+9.
 * https://oeis.org/A230349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3+20*n^2+110*n+109) * ∏_{k=n+12}^{\,2n+9} k (with
 *            a(0)=1, a(1)=20). For n≥2 the product is empty for n=2, yielding
 *            the correct value 417, and grows accordingly for larger n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;

    /* compute the polynomial part */
    seq_t sn  = (seq_t)n;
    seq_t poly = sn * sn * sn                 /* n^3         */
               + (seq_t)20 * sn * sn           /* 20*n^2      */
               + (seq_t)110 * sn              /* 110*n       */
               + (seq_t)109;                  /* +109        */

    /* compute the factorial ratio as a product */
    idx_t start = n + 12;               /* n+12        */
    idx_t end   = 2 * n + 9;            /* 2n+9        */
    seq_t ratio = 1;
    for (idx_t k = start; k <= end; ++k)
        ratio *= (seq_t)k;

    return poly * ratio;
}
