/*
 * A210373  Number of 2 X 2 matrices with all elements in {0,1,...,n} and
 *          positive odd determinant.
 * https://oeis.org/A210373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1 -(-1)^n)^2*(6*n + 5 -(-1)^n)*(2*n + 3 +
 *            (-1)^n)/256 Equivalent case split: If n is even: a(n) =
 *            n^2*(6*n+4)*(n+2)/32 If n is odd: a(n) = 3*(n+1)^4/16 For n >= 0
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 388, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use unsigned long long for intermediate to avoid overflow up to n≈10000 */
    if ((n & 1ULL) == 0) {                /* n even */
        unsigned long long nn = n;           /* cast to 64‑bit */
        unsigned long long num = nn * nn;   /* n^2 */
        num *= (6 * nn + 4);                /* * (6n+4) */
        num *= (nn + 2);                    /* * (n+2) */
        return (seq_t)(num / 32);           /* divide by 32 */
    } else {                              /* n odd */
        unsigned long long m = n + 1;       /* m = n+1 */
        unsigned long long num = m * m;     /* m^2 */
        num *= num;                        /* m^4 */
        num *= 3;                          /* *3 */
        return (seq_t)(num / 16);           /* divide by 16 */
    }
}
