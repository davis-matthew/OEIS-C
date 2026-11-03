/*
 * A154271  Dirichlet inverse of A154272; Fully multiplicative with a(3^e) =
 *          (-1)^e, a(p^e) = 0 for primes p <> 3.
 * https://oeis.org/A154271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   not stated
 * Range:     i8 n <= 59049, i16 n <= 59049, i32 n <= 59049, i64 n <= 59049,
 *            i128 n <= 59049, big any n
 * Validated: standard, pass, 111 terms through n = 9854 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fully multiplicative: a(3) = -1, a(p) = 0 for primes p ≠ 3 */
seq_t a(idx_t n)
{
    idx_t temp = n;
    idx_t e = 0;
    while ((temp % 3) == 0) {
        ++e;
        temp /= 3;
    }
    if (temp != 1) {
        return (seq_t)0;
    }
    return (e & 1) ? (seq_t)-1 : (seq_t)1;
}
