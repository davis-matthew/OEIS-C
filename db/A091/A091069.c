/*
 * A091069  Moebius mu sequence for real quadratic extension sqrt(2).
 * https://oeis.org/A091069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, sign
 * Formula:   a(n) = 0 if n is even or n has a squared prime factor; otherwise
 *            a(n) = ∏_{p|n, p odd} (2 - |p (mod 8)|), i.e. a(n) = 1 if n ≡ ±1
 *            (mod 8) and -1 if n ≡ ±3 (mod 8) (for odd square‑free n). a(1) =
 *            1
 * Range:     i8 n <= 9878, i16 n <= 9878, i32 n <= 9878, i64 n <= 9878,
 *            i128 n <= 9878, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A091069 (Moebius mu sequence for sqrt(2)). */
seq_t a(idx_t n)
{
    if (n == 1)               // empty product
        return 1;

    /* Zero for any even n (2 is a square in Q(√2)). */
    if ((n & 1) == 0)
        return 0;

    seq_t sign = 1;
    idx_t tmp = n;

    /* Trial division by odd numbers. */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
                if (exp > 1)          /* squared prime factor */
                    return 0;
            }
            /* Determine sign contribution of the prime p. */
            idx_t r = p % 8;
            if (r == 1 || r == 7)      /* p ≡ ±1 (mod 8)  → factor +1 */
                sign *= 1;
            else                      /* p ≡ ±3 (mod 8)  → factor -1 */
                sign *= -1;
        }
    }

    /* If an odd prime > sqrt(original n) remains. */
    if (tmp > 1) {
        idx_t r = tmp % 8;
        if (r == 1 || r == 7)
            sign *= 1;
        else
            sign *= -1;
    }

    return sign;
}
