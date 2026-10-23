/*
 * A160498  Number of cubic primitive Dirichlet characters modulo n.
 * https://oeis.org/A160498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(1) = 1; For n ≥ 2, a(n) = ∏_{p^e‖n} g(p,e), where g(p,e) = 2
 *            if (p == 3 and e == 2) or (p % 3 == 1 and e == 1); otherwise
 *            g(p,e) = 0. Thus a(n) is zero unless every prime power dividing
 *            n is either 3² or a prime ≡ 1 (mod 3) to the first power
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;                     /* a(1) = 1 */

    uint64_t temp = (uint64_t)n;
    seq_t result = 1;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            if (p == 3) {
                if (e != 2) return 0;       /* 3^e with e ≠ 2 gives 0 */
                result *= 2;
            } else {
                if (e != 1) return 0;       /* non‑squarefree factor → 0 */
                if (p % 3 != 1) return 0;   /* prime not ≡1 (mod 3) → 0 */
                result *= 2;
            }
        }
    }

    /* Remaining prime factor (if any) has exponent 1 */
    if (temp > 1) {
        uint64_t p = temp;
        if (p == 3) return 0;                /* 3^1 gives 0 */
        if (p % 3 != 1) return 0;            /* prime not ≡1 (mod 3) → 0 */
        result *= 2;
    }

    return result;
}
