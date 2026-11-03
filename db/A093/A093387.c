/*
 * A093387  a(n) = 2^(n-1) - binomial(n, floor(n/2)).
 * https://oeis.org/A093387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 2^(n-1) - binomial(n, floor(n/2)) (offset 1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 999 (2026-11-01)
 * Helpers:   binomial, binomial_idx_to_u128, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the term for any n ≥ 1.  For 64‑bit builds the calculation
   uses 128‑bit arithmetic to avoid overflow; for arbitrary‑precision
   builds it uses the existing big_pow routine. */

#include <stdint.h>
#if !defined(__SIZEOF_INT128__) && !(defined(__GNUC__) || defined(__clang__))
/* If __int128 is not available, the 64‑bit branch will not be taken. */
#endif
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/binomial_idx_to_u128.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* defined for completeness */
    idx_t k = n >> 1;                  /* floor(n/2) */

    /* ---------- 64‑bit seq_t branch ---------- */
    if (sizeof(seq_t) == 8) {
        /* For 64‑bit seq_t we rely on 128‑bit intermediates.  The
           sequence values fit into 64 bits for n ≤ 65; for larger n
           the result cannot be represented and we return 0. */
        if (n > 65) return 0;          /* 2^(n-1) would overflow 128‑bit */
        unsigned __int128 bin = binomial_idx_to_u128(n, k);
        unsigned __int128 pow2 = (unsigned __int128)1 << (n - 1);
        unsigned __int128 diff = pow2 - bin;
        return (seq_t)diff;
    }

    /* ----------  arbitrary‑precision seq_t branch ---------- */
    /* For non‑built‑in seq_t use the provided big_pow.               */
    seq_t bin = binomial(n, k);
    seq_t pow2 = ipow((seq_t)2, (seq_t)(n - 1));
    return pow2 - bin;
}
