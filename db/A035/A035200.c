/*
 * A035200  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          18.
 * https://oeis.org/A035200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(18, d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative calculation:  a(p^e) = 
   1                     if p=2 or p=3,
   (1+(-1)^e)/2          if Kronecker(18,p) = -1  (p ≡ 3,5 mod 8),
   e+1                   if Kronecker(18,p) =  1  (p ≡ 1,7 mod 8). */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            seq_t factor;
            if (p == 2 || p == 3) {
                factor = 1;
            } else {
                unsigned r = (unsigned)(p % 8);
                int k = (r == 1 || r == 7) ? 1 : -1;
                if (k == 1) {
                    factor = (seq_t)(e + 1);
                } else { /* k == -1 */
                    factor = (e % 2 == 0) ? (seq_t)1 : (seq_t)0;
                }
            }
            result *= factor;
        }
    }

    /* Remaining prime factor, if any. */
    if (temp > 1) {
        idx_t p = temp;          /* exponent is 1 */
        seq_t factor;
        if (p == 2 || p == 3) {
            factor = 1;
        } else {
            unsigned r = (unsigned)(p % 8);
            int k = (r == 1 || r == 7) ? 1 : -1;
            if (k == 1) {
                factor = 2;          /* e = 1 → e+1 = 2 */
            } else {                /* k == -1, e = 1 odd */
                factor = 0;
            }
        }
        result *= factor;
    }

    return result;
}
