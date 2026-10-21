/*
 * A061019  Negate primes in factorization of n.
 * https://oeis.org/A061019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, sign, mult
 * Formula:   a(n) = n*lambda(n), lambda(n)=(-1)^{Ω(n)} (Liouville's function)
 * Range:     i8 n <= 128, i16 n <= 9886, i32 n <= 9886, i64 n <= 9886,
 *            i128 n <= 9886, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n==0 in case of out‑of‑range calls – the sequence is defined only for n>=1 */
    if (n == 0) return 0;

    idx_t tmp = n;
    /* Count prime factors via trial division, flipping the sign for each occurrence. */
    int sign = 1;

    /* Handle factor 2 specially for speed. */
    while ((tmp & 1) == 0) {
        sign = -sign;
        tmp >>= 1;
    }

    /* Iterate over odd potential divisors up to sqrt(tmp). */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        while (tmp % p == 0) {
            sign = -sign;
            tmp /= p;
        }
    }

    /* If anything remains, it is a prime factor > sqrt(original n). */
    if (tmp > 1)
        sign = -sign;

    /* Multiply the original n by the sign to obtain a(n). */
    return (seq_t)n * sign;
}
