/*
 * A123331  Expansion of (c(q)^2/(3c(q^2))-1)/2 in powers of q where c(q) is a
 *          cubic AGM function.
 * https://oeis.org/A123331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative definition
   a(2^e) = (3-(-1)^e)/2  (i.e., 1 if e even, 2 if e odd)
   a(3^e) = 1
   a(p^e) = e+1           if p ≡ 1 (mod 6)
   a(p^e) = (1+(-1)^e)/2  if p ≡ 5 (mod 6)   (0 if e odd)
   and a is multiplicative */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* offset not used, just guard */
    seq_t result = 1;
    idx_t m = n;

    /* handle factor 2 */
    if ((m & 1) == 0) {
        idx_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            e++;
        }
        if (e & 1)          /* e odd -> factor 2 */
            result *= (seq_t)2;
        /* e even -> factor 1, do nothing */
    }

    /* handle factor 3 */
    if (m % 3 == 0) {
        idx_t e = 0;
        while (m % 3 == 0) {
            m /= 3;
            e++;
        }
        /* factor 1, nothing to do */
    }

    /* handle odd primes >=5 */
    for (idx_t p = 5; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            if (p % 6 == 1) {               /* p ≡ 1 (mod 6) */
                result *= (seq_t)(e + 1);
            } else if (p % 6 == 5) {        /* p ≡ 5 (mod 6) */
                if (e & 1)                  /* odd exponent -> 0 */
                    return 0;
                /* even exponent -> factor 1, do nothing */
            }
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        if (m % 6 == 1) {
            result *= (seq_t)2;   /* exponent = 1 -> e+1 = 2 */
        } else if (m % 6 == 5) {
            /* exponent 1 odd -> 0 */
            return 0;
        } /* else m is 2 or 3 which are already handled */
    }

    return result;
}
