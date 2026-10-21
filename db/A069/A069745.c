/*
 * A069745  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^6)*(1-x^7)*(1-x^8)).
 * https://oeis.org/A069745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A069745: a(n) = floor((n^4+52*n^3+996*n^2+7424*n+37632)/48384 
   - (n mod 2)*(n+2)*(n+24)/384 + ((n+2) mod 3)*(n+6)/54) */
/* The implementation evaluates the whole expression with a common
   denominator 48384, avoiding floating‑point operations. */

seq_t a(idx_t n)
{
    /* Use 64‑bit unsigned integers for the intermediate calculations.
       The intermediate values do not exceed 2^64 for the input ranges
       that are tested. */
    unsigned long long nn = n;          /* copy to a native type */
    unsigned long long n2 = nn * nn;    /* n^2 */
    unsigned long long n3 = n2 * nn;    /* n^3 */
    unsigned long long n4 = n3 * nn;    /* n^4 */

    /* numerator of the common denominator 48384 */
    unsigned long long N = n4
                         + 52ULL * n3
                         + 996ULL * n2
                         + 7424ULL * nn
                         + 37632ULL;

    /* subtract the contribution of the (n mod 2) term (multiplied by 126
       because 48384/384 = 126) */
    if (nn & 1ULL) {
        unsigned long long sub = (nn + 2ULL) * (nn + 24ULL) * 126ULL;
        N -= sub;
    }

    /* add the contribution of the ((n+2) mod 3) term (multiplied by 896
       because 48384/54 = 896) */
    unsigned long long mod3 = (nn + 2ULL) % 3ULL;
    if (mod3) {                      /* mod3 is 1 or 2 */
        unsigned long long add = mod3 * (nn + 6ULL) * 896ULL;
        N += add;
    }

    /* final floor division by 48384 */
    seq_t result = (seq_t)(N / 48384ULL);
    return result;
}
