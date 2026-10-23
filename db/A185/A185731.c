/*
 * A185731  Array by antidiagonals: T(n,k)=F*(k*n-n+3*k+13), where F =
 *          k*(k+1)*(k+2)*n*(n+1)*(n+2)/576.
 * https://oeis.org/A185731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n,k)=k*(k+1)*(k+2)*n*(n+1)*(n+2)*(k*n - n + 3*k +
 *            13)/576
 * Range:     u8 n <= 16, u16 n <= 110, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Antidiagonal enumeration with offset 1 */

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1 (offset 1) */
    if (n == 0) return 0;

    /* find the antidiagonal d such that d*(d-1)/2 >= n */
    idx_t d = 1;
    idx_t cum = 0;
    while (cum < n) {
        ++d;
        cum += d - 1; /* add size of current antidiagonal */
    }

    /* offset within the d‑th antidiagonal (1 … d-1) */
    idx_t prev_cum = cum - (d - 1);
    idx_t offset    = n - prev_cum;            /* row index in the array (1‑based) */
    idx_t k         = d - offset;              /* column index in the array (1‑based) */
    idx_t row       = offset;

    /* use 64‑bit intermediate arithmetic to avoid overflow, then cast */
    unsigned long long Ku = (unsigned long long)k;
    unsigned long long Nu = (unsigned long long)row;

    unsigned long long A = Ku * (Ku + 1) * (Ku + 2) / 6;   /* C(k+2,3) */
    unsigned long long B = Nu * (Nu + 1) * (Nu + 2) / 6;   /* C(n+2,3) */

    unsigned long long inner = Ku * Nu - Nu + 3 * Ku + 13;

    unsigned long long full = A * B * inner;               /* numerator before division by 16 */
    unsigned long long result_u = full / 16;               /* denominator is 16 */

    return (seq_t)result_u;
}
