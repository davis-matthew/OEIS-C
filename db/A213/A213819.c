/*
 * A213819  Rectangular array: (row n) = b**c, where b(h) = h, c(h) =
 *          3*n-4+3*h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained by enumerating the rectangular array formed by
 *            the convolution of b(h)=h and c(h)=3*n-4+3*h. For a given index
 *            n (1‑based) we determine the unique antidiagonal `d` such that
 *            (d-1)d/2 < n ≤ d(d+1)/2. Within this diagonal the row index r =
 *            n - (d-1)d/2 (r ranges from 1 to d) and th...
 * Range:     u8 n <= 28, u16 n <= 1275, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard anyway */

    /* find diagonal d with triangular numbers */
    idx_t d = 1;
    while ( (d * (d + 1)) / 2 < n )
        ++d;

    idx_t prev = (d - 1) * d / 2;   /* total elements before diagonal d */
    idx_t r    = n - prev;          /* row index within diagonal (1‑based) */
    idx_t k    = d + 1 - r;         /* column index (1‑based) */

    /* compute t_k(r) = k*(k+1)*(3*r + k - 2)/2 avoiding overflow
       by dividing an even factor by 2 first */
    seq_t k1 = (seq_t)k;
    seq_t k2 = (seq_t)(k + 1);
    if ( (k1 & 1) == 0 )
        k1 >>= 1;
    else
        k2 >>= 1;

    seq_t mul = k1 * k2;                /* k*(k+1)/2 */
    seq_t inner = (seq_t)(3 * r + k - 2);
    return mul * inner;
}
