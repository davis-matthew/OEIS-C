/*
 * A154269  Dirichlet inverse of A019590; Fully multiplicative with a(2^e) =
 *          (-1)^e, a(p^e) = 0 for odd primes p.
 * https://oeis.org/A154269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = 1 if n is 2^(even exponent), -1 if n is 2^(odd exponent),
 *            0 otherwise (multiplicative, a(1)=1)
 * Range:     i8 n <= 9452, i16 n <= 9452, i32 n <= 9452, i64 n <= 9452,
 *            i128 n <= 9452, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return 0 for all non‑powers of two */
    if ((n & (n - 1)) != 0) {
        return (seq_t)0;
    }

    /* count exponent e such that n = 2^e */
    idx_t e = 0;
    while (n > 1) {
        n >>= 1;
        ++e;
    }

    /* (-1)^e: 1 if e even, -1 if e odd */
    return (seq_t)((e & 1) ? -1 : 1);
}
