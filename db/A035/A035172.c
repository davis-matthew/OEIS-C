/*
 * A035172  a(n) = Sum_{d|n} Kronecker(-18, d).
 * https://oeis.org/A035172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-18, d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* index 0 is out of bounds for the OEIS offset */

    seq_t result = 1;
    idx_t temp = n;

    /* factor 2 */
    if (temp % 2 == 0) {
        while (temp % 2 == 0) temp /= 2;
        /* a(2^e) = 1, so result stays 1 */
    }

    /* factor 3 */
    if (temp % 3 == 0) {
        while (temp % 3 == 0) temp /= 3;
        /* a(3^e) = 1 */
    }

    /* remaining prime factors */
    for (idx_t p = 5; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            seq_t factor;
            if ((p % 8 == 1) || (p % 8 == 3)) {
                factor = (seq_t)(e + 1);          /* e+1  */
            } else { /* p % 8 == 5 or 7 */
                factor = (e % 2 == 0) ? (seq_t)1 : (seq_t)0; /* (1+(-1)^e)/2 */
            }
            result *= factor;
            if (result == 0) break;   /* shortcut: product is zero */
        }
    }

    /* if temp > 1, it is a remaining prime factor */
    if (temp > 1) {
        uint64_t p = temp;
        uint64_t e = 1;                /* exponent is 1 */
        seq_t factor;
        if ((p % 8 == 1) || (p % 8 == 3)) {
            factor = (seq_t)(e + 1);
        } else {
            factor = (e % 2 == 0) ? (seq_t)1 : (seq_t)0;
        }
        result *= factor;
    }

    return result;
}
