/*
 * A057222  Number of 4 X n binary matrices with exactly 1 singleton column up
 *          to row and column permutations.
 * https://oeis.org/A057222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/6*( binom(n+10,11) + Σ_{i=0}^{⌊(n-1)/3⌋}
 *            (i+2)(i+1)(n-1-3i+2)(n-1-3i+1)/2 + Σ_{i=0}^{⌊(n-1)/2⌋}
 *            (i+2)(i+1)*3* binom(n-1-2i+5,5)/2 )
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, values for n<1 do not matter */
    idx_t k = n - 1;                /* use k = n-1 for the generating‑function index */

    /* Sum A : C(11+k,11) */
    __int128 sumA = 1;
    for (idx_t j = 1; j <= 11; ++j) {
        sumA = sumA * (k + j);
        sumA = sumA / j;
    }

    /* Sum B : 2 * Σ binom(2+i,i) * binom(k-3i+2,2)   ->   Σ (i+2)(i+1)(k-3i+2)(k-3i+1)/2 */
    __int128 sumB = 0;
    for (idx_t i = 0; 3 * i <= k; ++i) {
        __int128 term1 = (__int128)(i + 2) * (i + 1);
        __int128 term2 = (__int128)(k - 3 * i + 2) * (k - 3 * i + 1);
        __int128 prod  = term1 * term2;
        prod /= 2;                 /* the division is exact */
        sumB += prod;
    }

    /* Sum C : 3 * Σ binom(2+i,i) * binom(5 + k-2i,5)  ->   Σ (i+2)(i+1)*3*binom(k-2i+5,5)/2 */
    __int128 sumC = 0;
    for (idx_t i = 0; 2 * i <= k; ++i) {
        __int128 term1 = (__int128)(i + 2) * (i + 1);          /* binom(2+i,i) = (i+2)(i+1)/2 */
        idx_t     m     = k - 2 * i;                          /* m = k-2i */
        /* binom(k-2i+5,5) computed multiplicatively */
        __int128 bin5  = (__int128)(m + 5) * (m + 4) * (m + 3) * (m + 2) * (m + 1);
        bin5 /= 120;                                         /* 5! = 120 */
        __int128 temp  = (__int128)3 * term1;
        temp /= 2;                   /* division by 2 is exact because term1 is even */
        temp *= bin5;
        sumC += temp;
    }

    __int128 total = sumA + sumB + sumC;
    return (seq_t)(total / 6);
}
