/*
 * A024573  a(n) = floor(1/frac(n*e)).
 * https://oeis.org/A024573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1/frac(n*e))
 * Range:     u8 n <= 393, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* e = 2.718281828459045…  scaled up by 10^15 (15 decimal digits) */
static const unsigned long long E_SCALE = 2718281828459045ULL;   /* floor(e*10^15) */
static const unsigned long long DEN     = 1000000000000000ULL;   /* 10^15 */

/* Compute a(n) for the OEIS sequence A024573 */
seq_t a(idx_t n)
{
    /* The fractional part of n*e is approximated as
     *    frac(n*e) ≈ ((n * E_SCALE) mod DEN) / DEN
     * Since e is irrational the remainder is never zero,
     * but we guard against this unlikely case.           */
    unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)E_SCALE;
    unsigned long long rem  = (unsigned long long)(prod % DEN);

    if (rem == 0)                /* safety fallback – should never happen */
        return (seq_t)0;

    unsigned long long val = DEN / rem;   /* floor(1/(frac…)) */
    return (seq_t)val;
}
