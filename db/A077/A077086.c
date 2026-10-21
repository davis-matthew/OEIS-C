/*
 * A077086  Remainder when sigma(n+1) is divided by sigma(n).
 * https://oeis.org/A077086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n+1) mod sigma(n) (OEIS A077086)
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _isqrt_u64, _sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/_isqrt_u64.h"
#include "../utils/_sigma_of.h"

/* ---------- public sequence function -------------------------------- */
seq_t a(idx_t n)
{
    /* n is 1-based as per OEIS; for n < 1 behaviour is undefined */
    seq_t sigma_n   = _sigma_of(n);
    seq_t sigma_np1 = _sigma_of(n + 1);
    /* sigma(n) > 0 for all n >= 1, but guard against division by zero */
    if (sigma_n == 0)
        return sigma_np1;
    return sigma_np1 % sigma_n;
}
