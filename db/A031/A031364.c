/*
 * A031364  Number of coincidence site modules of index 10n+1 in an
 *          icosahedral module.
 * https://oeis.org/A031364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is multiplicative. For n = ∏p^e, let * a(p^e) = 1 if e = 0,
 *            * a(5^e) = 5^e + 5^(e-1) (e ≥ 1), * a(p^e) = (e+1)p^e + 2e
 *            p^(e-1) + (e-1)p^(e-2) * for p ≡ ±1 (mod 5), e ≥ 1, * a(p^e) = 0
 *            if p ≡ ±2 (mod 5) and e odd, * a(p^e) = p^(e-2)(p^2+1) for p ≡
 *            ±2 (mod 5) and e even, e ≥ 2; * (in all other...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include helpers */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/factor_u64.h"

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, fallback */
    /* factorise n (n fits in uint64_t) */
    uint64_t primes[32], exps[32];
    size_t    cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t    e = (idx_t)exps[i];
        if (e == 0) continue;     /* contributes a factor 1 */

        seq_t local;

        if (p == 5) { /* special rule for 5 */
            seq_t pow5e   = seq_pow((seq_t)5, e);
            seq_t pow5e_1 = seq_pow((seq_t)5, e - 1);
            local = pow5e + pow5e_1;
        } else {
            uint64_t mod5 = p % 5;
            if (mod5 == 1 || mod5 == 4) { /* p ≡ ±1 (mod 5) */
                seq_t pe  = seq_pow((seq_t)p, e);      /* p^e   */
                seq_t pe1 = seq_pow((seq_t)p, e - 1);  /* p^(e‑1) */
                local = (seq_t)(e + 1) * pe
                      + (seq_t)(2 * e)   * pe1;
                if (e >= 2) {
                    seq_t pe2 = seq_pow((seq_t)p, e - 2); /* p^(e‑2) */
                    local += (seq_t)(e - 1) * pe2;
                }
            } else { /* p ≡ ±2 (mod 5) */
                if (e & 1) return 0ULL;              /* odd exponent → 0 */
                if (e == 0) { local = 1; }            /* never reached */
                else { /* e even and ≥2 */
                    seq_t pe2 = seq_pow((seq_t)p, e - 2); /* p^(e‑2) */
                    seq_t p2  = (seq_t)p * (seq_t)p;
                    local = pe2 * (p2 + (seq_t)1);
                }
            }
        }
        result *= local;
    }
    return result;
}
