/*
 * A128580  Expansion of phi(x^3) * psi(x^4) - x * phi(x) * psi(x^12) in
 *          powers of x where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A128580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = b(2*n + 1) where b() is multiplicative with * b(2^e) =
 *            0^e * b(3^e) = (-1)^e * b(p^e) = e+1 if p ≡ 1,7 (mod 24) *
 *            b(p^e) = (e+1)*(-1)^e if p ≡ 5,11 (mod 24) * b(p^e) =
 *            (1+(-1)^e)/2 if p ≡ 13,17,19,23 (mod 24)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = 2*n + 1 is always odd */
    idx_t m = 2 * n + 1;
    if (m == 1)
        return (seq_t)1; /* b(1) = 1 */

    /* Use a signed 64‑bit temporary: the values fit easily for n ≤ 10000 */
    int64_t temp = 1;
    idx_t t = m;

    /* Factorization by trial division */
    for (idx_t p = 3; p <= t / p; p += 2)
    {
        if (t % p == 0)
        {
            idx_t e = 0;
            while (t % p == 0)
            {
                t /= p;
                ++e;
            }

            int64_t factor;
            if (p == 3)
            {
                factor = (e & 1) ? -1 : 1;          /* (-1)^e */
            }
            else
            {
                uint8_t mod = p % 24;
                if (mod == 1 || mod == 7)
                {
                    factor = (int64_t)(e + 1);        /* e+1 */
                }
                else if (mod == 5 || mod == 11)
                {
                    factor = ((e & 1) ? -(int64_t)(e + 1)
                                      :  (int64_t)(e + 1));
                }
                else if (mod == 13 || mod == 17 ||
                         mod == 19 || mod == 23)
                {
                    factor = (e & 1) ? 0 : 1;          /* (1+(-1)^e)/2 */
                }
                else
                {
                    /* The modulus 24 cannot be any other value for any prime > 3 */
                    factor = 1;
                }
            }

            temp *= factor;
            if (temp == 0)
                break;   /* b(m) is zero, no need to continue */
        }
    }

    /* Remaining prime factor > sqrt(t) */
    if (temp != 0 && t > 1)
    {
        /* Remaining prime p = t, exponent e = 1 */
        idx_t p = t;
        int64_t factor;
        if (p == 3)
        {
            factor = -1;
        }
        else
        {
            uint8_t mod = p % 24;
            if (mod == 1 || mod == 7)
                factor = 2;          /* e+1 with e = 1 */
            else if (mod == 5 || mod == 11)
                factor = -2;         /* (e+1)*(-1)^e = 2*(-1) */
            else if (mod == 13 || mod == 17 ||
                     mod == 19 || mod == 23)
                factor = 0;          /* (1+(-1))/2 = 0 */
            else
                factor = 1;          /* Should not occur */
        }
        temp *= factor;
    }

    return (seq_t)temp;
}
