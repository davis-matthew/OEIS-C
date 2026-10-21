/*
 * A206811  Sum_{0<j<k<=n} (k^4-j^4).
 * https://oeis.org/A206811
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n+1)^2*(1-6*n+n^2+4*n^3))/30
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 56, u64 n <= 2273,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* out of offset range */

    seq_t nn  = (seq_t)n;                 /* n */
    seq_t n2  = nn * nn;                 /* n^2 */
    seq_t n3  = n2 * nn;                 /* n^3 */

    seq_t num1 = nn;                     /* n                 */
    seq_t num2 = nn + (seq_t)1;          /* n+1               */
    num2 = num2 * num2;                  /* (n+1)^2           */
    seq_t num3 = (seq_t)1                /* 1                 */
               - (seq_t)6 * nn            /* -6n               */
               + n2                      /* +n^2              */
               + (seq_t)4 * n3;           /* +4n^3             */

    seq_t den = (seq_t)30;               /* denominator */

    const int primes[3] = {2, 3, 5};
    for (int i = 0; i < 3; ++i) {
        int p = primes[i];
        while (den % p == 0) {
            if (num1 % p == 0) { num1 /= p; den /= p; continue; }
            if (num2 % p == 0) { num2 /= p; den /= p; continue; }
            if (num3 % p == 0) { num3 /= p; den /= p; continue; }
            /* denominator factors should always cancel; break if not */
            break;
        }
    }

    /* final product */
    seq_t prod = num1 * num2;
    prod = prod * num3;

    /* if any factor of denominator remained (unlikely) divide */
    if (den > 1) prod /= den;

    return prod;
}
