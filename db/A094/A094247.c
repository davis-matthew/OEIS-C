/*
 * A094247  Expansion of (phi(-q^5)^2 - phi(-q)^2) / 4 in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A094247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   A094247 is a multiplicative arithmetic function defined by: -
 *            a(2^e) = -1 (for any e > 0) - a(5^e) = 1 (for any e > 0) - if p
 *            % 8 == 1 or 5: a(p^e) = e + 1 - if p % 8 == 3 or 7: a(p^e) = (1
 *            + (−1)^e) / 2 (i.e. 1 if e even, 0 if e odd) Thus a(n) = ∏ₚ
 *            a(p^{e_p})
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but return 0 to be safe */
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t f;
            if (p == 2) {
                /* a(2^e) = -1 for any e>0 */
                f = -1;
            } else if (p == 5) {
                f = 1;
            } else {
                uint8_t mod8 = p & 7;   /* p % 8 */
                if (mod8 == 1 || mod8 == 5) {
                    f = (seq_t)(exp + 1);
                } else { /* mod8 == 3 or 7 */
                    f = (exp & 1) ? 0 : 1;
                }
            }
            result *= f;
            if (result == 0) return 0;   /* early exit */
        }
    }
    if (m > 1) {   /* remaining prime factor */
        seq_t f;
        idx_t p = m;
        idx_t exp = 1;
        if (p == 2) {
            f = -1;
        } else if (p == 5) {
            f = 1;
        } else {
            uint8_t mod8 = p & 7;
            if (mod8 == 1 || mod8 == 5) {
                f = exp + 1;   /* exp is 1 */
            } else {
                f = 1;         /* exp is odd, gives 0? wait odd => 0 */
                /* For p % 8 ==3 or 7 and e=1 (odd) => f=0 */
                f = 0;
            }
        }
        result *= f;
    }
    return result;
}
