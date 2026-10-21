/*
 * A229143  Expansion of (b(q^3) - b(q)) / 3 in powers of q where b() is a
 *          cubic AGM theta function.
 * https://oeis.org/A229143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   not stated
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A229143: multiplicative law
   a(3)   = -3,   a(3^e) = 0 (e>1)
   a(p^e) = e+1     if p ≡ 1 (mod 3)
   a(p^e) = (1+(-1)^e)/2  if p ≡ 2 (mod 3)
   (i.e. non‑zero only when every p≡2(mod 3) has even exponent,
   and the factor from 3 is -3 if a single 3 is present.) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out of bounds, unused */
    idx_t temp = n;
    seq_t result = 1;

    /* factor out powers of 3 */
    idx_t exp3 = 0;
    while (temp % 3 == 0) {
        ++exp3;
        temp /= 3;
    }
    if (exp3 > 1) return 0;        /* 3^e with e>1 gives zero */
    if (exp3 == 1) result = -3;    /* factor from a single 3 */

    /* factor remaining part */
    idx_t p = 2;
    while (p * p <= temp) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (p % 3 == 1) {            /* p ≡ 1 (mod 3) */
                result *= (seq_t)(e + 1);
            } else if (p % 3 == 2) {     /* p ≡ 2 (mod 3) */
                if (e & 1) return 0;    /* odd exponent → zero */
                /* even exponent → factor 1, do nothing */
            }
        }
        ++p;
    }

    /* possible remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        if (temp % 3 == 1) {
            result *= 2;                 /* exponent 1 → e+1 = 2 */
        } else if (temp % 3 == 2) {
            return 0;                    /* odd exponent → zero */
        }
    }

    return result;
}
