/*
 * A217220  Theta series of Kagome net with respect to an atom.
 * https://oeis.org/A217220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 4 * b(n) where b is multiplicative with * b(2^e) =
 *            (1+(-1)^e)*3/4 (e≥1), * b(3^e) = 1, * b(p^e) = (1+(-1)^e)/2 when
 *            p≡5 (mod 6), * b(p^e) = e+1 when p≡1 (mod 6). * a(0) = 1
 * Range:     u8 n <= 9881, u16 n <= 9881, u32 n <= 9881, u64 n <= 9881,
 *            u128 n <= 9881, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t nn = n;

    /* factor out powers of 2 */
    idx_t e2 = 0;
    while ((nn & 1) == 0) {
        e2++;
        nn >>= 1;
    }
    if (e2 % 2 == 1)        /* odd power => 0 */
        return (seq_t)0;
    int two_even = (e2 >= 2);   /* true if power of 2 is ≥2 and even */

    seq_t f = (seq_t)1;      /* multiplicative part from primes other than 2 */

    /* trial division for remaining odd factors */
    idx_t p = 3;
    while (p <= nn / p) {    /* avoid overflow */
        if (nn % p == 0) {
            idx_t exp = 0;
            while (nn % p == 0) {
                nn /= p;
                exp++;
            }
            if (p == 3) {
                /* no change to f */
            } else if (p % 6 == 5) {
                if (exp % 2 == 1)
                    return (seq_t)0;
                /* else multiply by 1 */
            } else if (p % 6 == 1) {
                f = f * (seq_t)(exp + 1);
            }
        }
        p += 2;
    }

    if (nn > 1) {             /* remaining prime factor */
        idx_t exp = 1;
        idx_t p2 = nn;
        if (p2 == 3) {
            /* no change */
        } else if (p2 % 6 == 5) {
            if (exp % 2 == 1)
                return (seq_t)0;
        } else if (p2 % 6 == 1) {
            f = f * (seq_t)(exp + 1);   /* exp is 1 => multiply by 2 */
        }
    }

    if (two_even)
        return (seq_t)6 * f;   /* 4 * (3/2) * f = 6 * f */
    else
        return (seq_t)4 * f;
}
