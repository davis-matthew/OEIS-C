/*
 * A076952  n-th power associated with A076951, or 0 if no such number exists.
 * https://oeis.org/A076952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m^n such that m^n ≡ -1 (mod n), or 0 if no such
 *            m exists
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 25,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* n = 1 has the special definition a(1) = 0 */
    if (n <= 1)
        return 0;

    /* n is small (≤10000 for the test harness) so we may search all residues 1 … n‑1 */
    uint64_t mod = (uint64_t)n;          /* use native 64‑bit arithmetic for the modular test */

    for (idx_t m = 1; m < n; ++m)
    {
        /* compute m^n mod n using 64‑bit arithmetic */
        uint64_t result = 1;
        uint64_t base   = (uint64_t)m % mod;
        idx_t    exp    = n;

        while (exp > 0)
        {
            if (exp & 1U)
                result = (result * base) % mod;
            base = (base * base) % mod;
            exp >>= 1U;
        }

        /* check for the congruence m^n ≡ -1 (mod n)  →  m^n mod n == n-1 */
        if (result == (mod - 1U))
        {
            /* return the exact value m^n in the required type seq_t */
            return pow_uint((uint64_t)m, n);
        }
    }

    /* no residue satisfies the congruence → the sequence value is 0 */
    return 0;
}
