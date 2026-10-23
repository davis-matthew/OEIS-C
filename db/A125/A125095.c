/*
 * A125095  Expansion of phi(-x) * psi(x^4) in powers of x where psi(), phi()
 *          are Ramanujan theta functions.
 * https://oeis.org/A125095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = b(2*n+1) where b is multiplicative. b(2^e) = 0^e, b(p^e)
 *            = (e+1)*(-1)^(((p-1)/2)*e) if p ≡ 1 or 3 (mod 8), b(p^e) =
 *            (1+(-1)^e)/2 if p ≡ 5 or 7 (mod 8)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = 2*n + 1 */
    idx_t m = (idx_t)2 * n + 1;
    seq_t prod = 1;
    idx_t tmp = m;

    for (idx_t p = 3; (seq_t)p * (seq_t)p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }

            seq_t val;
            int mod8 = p & 7;
            if (mod8 == 1 || mod8 == 3) {
                int sign = 1;
                /* exponent for sign: ((p-1)/2)*e */
                if (((p - 1) >> 1) & 1) {   /* (p-1)/2 is odd */
                    if (e & 1) {
                        sign = -1;
                    }
                }
                val = (seq_t)(e + 1) * (seq_t)sign;
            } else { /* mod8 == 5 or 7 */
                /* (1 + (-1)^e)/2 */
                val = ((e & 1) == 0) ? 1 : 0;
            }

            prod *= val;
            if (prod == 0) {
                break;
            }
        }
    }

    if (tmp > 1) {           /* remaining prime factor */
        int mod8 = tmp & 7;
        if (mod8 == 1 || mod8 == 3) {
            int sign = 1;
            if (((tmp - 1) >> 1) & 1) {
                /* exponent e = 1 is odd */
                sign = -1;
            }
            prod *= 2 * (seq_t)sign;   /* (e+1)=2 for e=1 */
        } else { /* mod8 == 5 or 7 */
            prod *= 0;   /* value is 0 */
        }
    }

    return prod;
}
