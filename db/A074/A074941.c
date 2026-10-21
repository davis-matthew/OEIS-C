/*
 * A074941  a(n) = sigma(n) mod 3.
 * https://oeis.org/A074941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n) mod 3
 * Range:     u8 n <= 8729, u16 n <= 8729, u32 n <= 8729, u64 n <= 8729,
 *            u128 n <= 8729, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Safety: idx_t is unsigned; n >= 1 for the OEIS sequence. */
    if (n == 0) return 0;  /* undefined but harmless */

    /* Small buffers for prime factorisation. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* Factorise n.  The helper writes into primes[], exps[], and cnt. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute: σ(n) mod 3 */
    seq_t result = 1;  /* multiplicative identity modulo 3 */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t pmod3 = p % 3;        /* 0,1,2 */

        seq_t sig_mod3;

        if (pmod3 == 0) {          /* p == 3 */
            sig_mod3 = 1;           /* 1 + 0 + 0 + … = 1 */
        } else if (pmod3 == 1) {
            /* σ(p^e) = (e+1) mod 3 */
            sig_mod3 = (seq_t)((e + 1) % 3);
        } else {                    /* pmod3 == 2 */
            /* σ(p^e) mod 3 = 1 if e is even, 0 if e is odd */
            sig_mod3 = (seq_t)((e % 2) == 0 ? 1 : 0);
        }

        result = (result * sig_mod3) % 3;
        if (result == 0) break;    /* early exit */
    }

    return result;
}
