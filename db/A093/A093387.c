/*
 * A093387  a(n) = 2^(n-1) - binomial(n, floor(n/2)).
 * https://oeis.org/A093387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 2^(n-1) - binomial(n, floor(n/2)) (offset 1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 30, u64 n <= 65, u128 n <= 128,
 *            big any n
 * Helpers:   big_pow, binom64, binom_seq
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
#include "../utils/big_pow.h"
#include "../utils/binom64.h"
#include "../utils/binom_seq.h"

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
        unsigned __int128 bin = binom64(n, k);
        unsigned __int128 pow2 = (unsigned __int128)1 << (n - 1);
        unsigned __int128 diff = pow2 - bin;
        return (seq_t)diff;
    }

    /* ----------  arbitrary‑precision seq_t branch ---------- */
    /* For non‑built‑in seq_t use the provided big_pow.               */
    seq_t bin = binom_seq(n, k);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n - 1));
    return pow2 - bin;
}
