/*
 * A036194  Log base 5 (n) mod 167.
 * https://oeis.org/A036194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 166, u16 n <= 166, u32 n <= 166, u64 n <= 166,
 *            u128 n <= 166, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = discrete logarithm of n to base 5 modulo 167
 * (i.e. find k with 5^k ≡ n (mod 167); if n≡0 (mod 167) a(n)=0)
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute discrete log table for residues 1…166 */
    static int logtab[167];
    static bool init = false;

    if (!init) {
        int prod = 1;
        logtab[1] = 0;             /* 5^0 ≡ 1 (mod 167) */

        /* 5 is a primitive root of 167, so its powers cycle through
         * all non‑zero residues exactly once.  The cycle closes after
         * 166 steps, but the residue 1 already has its minimal exponent 0,
         * so we stop at i = 165. */
        for (idx_t i = 1; i < 166; ++i) {
            prod = (prod * 5) % 167;   /* 5^i modulo 167 */
            logtab[prod] = (int)i;     /* store exponent i */
        }
        init = true;
    }

    idx_t mod = n % 167;
    if (mod == 0)
        return (seq_t)0;          /* convention for multiples of 167 */

    return (seq_t)logtab[(int)mod];
}
