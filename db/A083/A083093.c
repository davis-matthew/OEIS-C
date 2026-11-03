/*
 * A083093  Triangle, read by rows, formed by reading Pascal's triangle
 *          (A007318) mod 3.
 * https://oeis.org/A083093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = C(r,k) mod 3 where r = ⌊(√(8n+1) – 1)/2⌋ (row index) k =
 *            n – r(r+1)/2 (position in row)
 * Range:     u8 n <= 7380, u16 n <= 7380, u32 n <= 7380, u64 n <= 7380,
 *            u128 n <= 7380, big any n
 * Validated: standard, pass, 121 terms through n = 6203 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the sequence A083093 – Pascal's triangle modulo 3, read
    row by row (offset 0).  Uses Lucas' theorem in base 3 to
    evaluate binomial coefficients modulo 3 efficiently. */

#include <stdint.h>
#include <stdbool.h>

/* Binomial coefficient C(r,k) modulo 3 using Lucas in base 3.
   Returns 0, 1, or 2.  All arithmetic is on 64‑bit values. */
static uint8_t pascal_mod3_row_col(uint64_t r, uint64_t k)
{
    uint8_t parity = 0;          /* parity of occurrences of 2 in product */
    while (r || k) {
        uint8_t rd = (uint8_t)(r % 3);
        uint8_t kd = (uint8_t)(k % 3);
        if (kd > rd) return 0;   /* C(r_i,k_i)=0 */
        if (rd == 2 && kd == 1) parity ^= 1;   /* multiply by 2 (≡−1 mod3) */
        r /= 3;
        k /= 3;
    }
    return parity ? 2 : 1;
}

/* Integer square root for values up to (8·n+1) where n is uint64_t.
   Returns floor(√v) for a 128‑bit integer v.  The upper bound 2^34 is
   sufficient for all 8·n+1 encountered in the problem domain. */
static uint64_t isqrt_u128(__int128 v)
{
    uint64_t low  = 0;
    uint64_t high = (uint64_t)1ULL << 34;   /* > sqrt(max(8·n+1))   */
    uint64_t res  = 0;

    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= v) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Compute row index r from n */
    uint64_t r = (isqrt_u128((__int128)8 * n + 1) - 1) / 2;

    /* Compute column index k */
    uint64_t k = n - (r * (r + 1)) / 2;

    /* Return the binomial coefficient modulo 3 as a seq_t */
    return (seq_t)pascal_mod3_row_col(r, k);
}
