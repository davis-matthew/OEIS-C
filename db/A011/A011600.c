/*
 * A011600  Legendre symbol (n,83).
 * https://oeis.org/A011600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,83). Compute via Euler's criterion:
 *            a(n) = n^41 (mod 83), mapped to 0 -> 0, 1 -> 1, any other
 *            residue -> -1. This satisfies the stated recurrence and
 *            equivalent formulas
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t MOD = 83;               /* prime modulus */
    idx_t n_mod = n % MOD;              /* reduce n modulo 83 */

    /* binary exponentiation of n_mod^41 mod 83 */
    idx_t result = 1;
    idx_t base   = n_mod;
    idx_t exp    = 41;
    while (exp > 0) {
        if (exp & 1U) {
            result = (result * base) % MOD;
        }
        base = (base * base) % MOD;
        exp >>= 1U;
    }

    /* map the residue to -1, 0, or 1 */
    if (result == 0) return (seq_t)0;
    if (result == 1) return (seq_t)1;
    return (seq_t)(-1);
}
