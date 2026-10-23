/*
 * A133692  Expansion of phi(-q) * phi(q^2) in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A133692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1. For n ≥ 1, a(n) = -2 * ∏_{p^e || n} b(p^e) where
 *            b(2^e) = -1, b(p^e) for p ≡ 1,3 (mod 8) = e + 1, b(p^e) for p ≡
 *            5,7 (mod 8) = (1 + (-1)^e) / 2 (i.e. 1 if e even, 0 if e odd).
 *            The product is over all prime powers dividing n
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(0) is defined to be 1; all other n are computed by prime factorisation. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* a(0) = 1 */

    idx_t t = n;
    seq_t prod = 1;            /* multiplicative accumulator */

    idx_t p = 2;
    while (p <= t / p) {       /* loop over possible prime divisors up to √t */
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                e++;
            }

            seq_t bval;
            if (p == 2) {
                bval = -1;   /* b(2^e) = -1 for all e > 0 */
            } else {
                idx_t mod8 = p & 7;   /* p (mod 8) */
                if (mod8 == 1 || mod8 == 3) {
                    bval = (seq_t)(e + 1);
                } else { /* mod8 == 5 or 7 */
                    bval = ((e & 1) == 0) ? 1 : 0;
                }
            }

            prod *= bval;
            if (prod == 0)
                break;       /* zero factor guarantees all further terms zero */
        }

        /* advance to next candidate divisor: 3,5,7,9,... skipping even numbers */
        p = (p == 2) ? 3 : p + 2;
    }

    /* t now is either 1 or a remaining prime factor > √original n */
    if (prod != 0 && t > 1) {
        idx_t pprime = t;        /* remaining prime factor with exponent 1 */
        seq_t bval;
        if (pprime == 2) {
            bval = -1;           /* (unreachable for t>1, but kept for completeness) */
        } else {
            idx_t mod8 = pprime & 7;
            if (mod8 == 1 || mod8 == 3) {
                bval = (seq_t)2; /* e=1 ⇒ e+1 = 2 */
            } else {           /* mod8 == 5 or 7 */
                bval = 0;        /* (1 + (-1)^1)/2 = 0 */
            }
        }
        prod *= bval;
    }

    return (seq_t)-2 * prod;    /* final scaling by -2 */
}
