/*
 * A036164  Log base 5 (n) mod 23.
 * https://oeis.org/A036164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = exponent k such that 5^k ≡ n (mod 23). For n ≡ 0 (mod 23)
 *            returns 0
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: full, pass, 22 terms through n = 22 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Static helper that works for any seq_t size. */
static seq_t log5_mod_23(idx_t n)
{
    /* Reduce n modulo 23 using the native index type. */
    idx_t nm = n % 23ULL;
    if (nm == 0ULL)
        return (seq_t)0;          /* Undefined for 0; choose 0 as sentinel. */

    /* Compute the discrete logarithm by brute force (only 22 possibilities). */
    seq_t r = (seq_t)nm;          /* residue in seq_t domain */
    seq_t pow = (seq_t)1;         /* 5^0 */
    for (int k = 0; k < 23; ++k)
    {
        if (pow == r)
            return (seq_t)k;     /* found exponent */
        pow = (pow * (seq_t)5) % (seq_t)23;
    }
    return (seq_t)0;              /* unreachable, but keeps the compiler happy */
}

seq_t a(idx_t n)
{
    return log5_mod_23(n);
}
