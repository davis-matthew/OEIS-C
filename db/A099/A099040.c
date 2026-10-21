/*
 * A099040  Riordan array (1, 2+2x).
 * https://oeis.org/A099040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 2^c * binomial(c, r‑c) * where r = ⌊(√(8n+1) − 1)/2⌋ * c
 *            = n − r(r+1)/2 * * This is the row‑wise flattening of the
 *            Riordan triangle * T(r,c) = 2^c ⋅ binomial(c, r‑c)
 * Range:     u8 n <= 32, u16 n <= 100, u32 n <= 317, u64 n <= 1162,
 *            u128 n <= 4254, big any n
 * Helpers:   binomial, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b); */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp); */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t a, seq_t b); */

/* Integer square‑root of a 64‑bit value.
 * Returns ⌊√x⌋ using binary search.  */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x, best = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            best = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return best;
}

/* Sequence A099040: a(n) */
seq_t a(idx_t n)
{
    /* Compute r = floor((sqrt(8n+1)-1)/2) */
    idx_t sq = isqrt_u64((unsigned __int128)n * 8 + 1);
    idx_t r = (sq - 1) / 2;

    /* Number of elements before row r */
    idx_t before = r * (r + 1) / 2;
    idx_t c = n - before;                /* column in row r */

    /* T(r,c) = 2^c * binomial(c, r-c) */
    seq_t pow_val = pow2(c);
    seq_t bin_val = binomial(c, r - c);
    return seq_mul(pow_val, bin_val);
}
