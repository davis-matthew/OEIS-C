/*
 * A129447  Expansion of psi(q) * psi(q^3) * phi(q^3) / phi(q) in powers of q
 *          where psi(), phi() are Ramanujan theta functions.
 * https://oeis.org/A129447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = b(2*n + 1) where * b is multiplicative with * b(2^e) =
 *            0^e, * b(3^e) = (-1)^e, * b(p^e) = e+1 if p == 1 (mod 4), *
 *            b(p^e) = (1+(-1)^e)/2 if p == 3 (mod 4). * This computation uses
 *            only integer arithmetic compatible with all * supported
 *            seq_t/idx_t types
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = 2*n + 1;          /* m is odd */
    seq_t res = 1;
    idx_t temp = m;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }

            if (p == 3) {
                if (e & 1) res = -res;          /* (-1)^e */
            } else if (p % 4 == 1) {
                res *= (seq_t)(e + 1);          /* (e+1) */
            } else if (p % 4 == 3) {
                if (e & 1) return 0;           /* odd exponent makes b zero */
                /* even exponent contributes factor 1 – no change */
            } else if (p == 2) {
                return 0;                       /* any exponent of 2 gives b=0 */
            }
        }
    }

    /* Remaining prime factor larger than sqrt(original m) */
    if (temp > 1) {
        idx_t p = temp;
        idx_t e = 1;                 /* single exponent left */

        if (p == 3) {
            if (e & 1) res = -res;
        } else if (p % 4 == 1) {
            res *= (seq_t)(e + 1);
        } else if (p % 4 == 3) {
            if (e & 1) return 0;
        } else if (p == 2) {
            return 0;
        }
    }

    return res;
}
