/*
 * A215550  Number of standard Young tableaux of shape [10n,10].
 * https://oeis.org/A215550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(10*n+10,10)*(10*n-9)/(10*n+1) for n>0, a(0)=0
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 37, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* 10*n */
    seq_t ten_n = (seq_t)10 * n;

    /* Numerators of the cancelled product:
       (10n-9)*(10n+2)*(10n+3)*...*(10n+10)  */
    seq_t numer[10];
    numer[0]   = ten_n - (seq_t)9;            /* 10n-9    */
    for (idx_t i = 1; i < 10; ++i)           /* 10n+2 .. 10n+10 */
        numer[i] = ten_n + (seq_t)(i + 1);

    /* Remaining denominator prime powers: 2^8 * 3^4 * 5^2 * 7 */
    int p2 = 8, p3 = 4, p5 = 2, p7 = 1;

    for (int j = 0; j < 10; ++j) {
        /* cancel factor 2 */
        while (p2 > 0 && numer[j] % (seq_t)2 == 0) {
            numer[j] /= (seq_t)2;
            p2--;
        }
        /* cancel factor 3 */
        while (p3 > 0 && numer[j] % (seq_t)3 == 0) {
            numer[j] /= (seq_t)3;
            p3--;
        }
        /* cancel factor 5 */
        while (p5 > 0 && numer[j] % (seq_t)5 == 0) {
            numer[j] /= (seq_t)5;
            p5--;
        }
        /* cancel factor 7 */
        while (p7 > 0 && numer[j] % (seq_t)7 == 0) {
            numer[j] /= (seq_t)7;
            p7--;
        }
    }

    /* All denominator factors should have been cancelled. */
    /* Compute the product of the remaining numerators. */
    seq_t result = 1;
    for (int j = 0; j < 10; ++j)
        result *= numer[j];

    return result;
}
