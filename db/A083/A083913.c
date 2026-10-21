/*
 * A083913  Number of divisors of n that are congruent to 3 modulo 10.
 * https://oeis.org/A083913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n that are ≡ 3 (mod 10)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but safe */
    /* factor n */
    uint64_t fac[20];
    int exp[20];
    int cnt = 0;
    factor_uint64((uint64_t)n, fac, exp, &cnt);

    /* counts of divisor residues modulo 10 */
    seq_t counts[10] = {0};
    counts[1] = 1;                 /* divisor 1 */

    for (int i = 0; i < cnt; ++i) {
        int p_mod = (int)(fac[i] % 10ULL);
        seq_t new_counts[10] = {0};
        seq_t power = 1;           /* p^0 mod 10 */
        for (int e = 0; e <= exp[i]; ++e) {
            for (int m = 0; m < 10; ++m) {
                if (counts[m] == 0) continue;
                int new_mod = (m * (int)(power % 10)) % 10;
                new_counts[new_mod] += counts[m];
            }
            power = (power * (seq_t)p_mod) % 10;  /* next power of p */
        }
        /* copy back to counts */
        for (int m = 0; m < 10; ++m) counts[m] = new_counts[m];
    }
    return counts[3];
}
