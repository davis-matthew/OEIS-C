/*
 * A173194  a(n) = -sin^2 (2*n*arccos n) = - sin^2 (2*n*arcsin n).
 * https://oeis.org/A173194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 0 for n = 0,1
 *              a(n) = (n^2-1) * U_{2*n-1}(n)^2   for n ≥ 2
 * where U_k(x) is the Chebyshev polynomial of the second kind,
 * defined by U_0(x)=1, U_1(x)=2x, U_{k+1}(x)=2x·U_k(x)-U_{k-1}(x).
 */

seq_t a(idx_t n)
{
    /* a(0) and a(1) are defined to be 0 */
    if (n <= 1)
        return (seq_t)0;

    /* Convert n to seq_t for the polynomial computations */
    seq_t x = (seq_t)n;
    /* We need U_{2n-1}(n) */
    idx_t m = 2 * n - 1;          /* m = 2n-1,  n ≥ 2, so m ≥ 3 */

    /* Compute U_0 and U_1 */
    seq_t U_prev = (seq_t)1;      /* U_0(x) */
    seq_t U_curr = (seq_t)2 * x;  /* U_1(x) */

    /* If m == 1 we would return U_curr, but m≥3 here */
    for (idx_t i = 2; i <= m; ++i) {
        /* U_next = 2x * U_curr - U_prev */
        seq_t temp = (seq_t)2 * x * U_curr;  /* 2x * U_curr */
        temp = temp - U_prev;                /* subtract U_prev */
        U_prev = U_curr;
        U_curr = temp;
    }
    seq_t U = U_curr;  /* U_{2n-1}(n) */

    /* factor = n^2 - 1 */
    seq_t factor = (seq_t)n * (seq_t)n - (seq_t)1;

    /* a(n) = (n^2-1) * U^2 */
    seq_t t = U * U;
    t = t * factor;
    return t;
}
