/*
 * A011630  Legendre symbol (n,241).
 * https://oeis.org/A011630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A011630: Legendre symbol (n,241)
 *  Formula: a(n) = n^120 (mod 241)   mapped to { -1, 0, +1 }
 *  a(0) = 0
 *  a(1…240) = ±1 according to quadratic residue modulo 241
 */
seq_t a(idx_t n)
{
    const idx_t   mod241 = 241;
    const seq_t   mod   = (seq_t)mod241;

    /* Reduce n modulo 241 */
    idx_t n_mod = n % mod241;
    if (n_mod == 0)
        return (seq_t)0;

    /* Fast binary exponentiation: compute n_mod^120 mod 241 */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)n_mod;
    idx_t exp    = 120;

    while (exp > 0) {
        if (exp & 1UL)
            result = (result * base) % mod;
        exp >>= 1UL;
        if (exp)
            base = (base * base) % mod;
    }

    /* Map 1 → +1, 240 → –1 (Legendre symbol) */
    if (result == (seq_t)1)
        return (seq_t)1;
    else   /* result == 240 */
        return (seq_t)-1;
}
