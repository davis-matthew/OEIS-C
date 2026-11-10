/*
 * A230599  Inverse permutation to A230598.
 * https://oeis.org/A230599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   inverse of A230598, which maps each block [2^k, 2^(k+1)) onto
 *            itself. a(1..7) = 1,3,2,5,6,4,7. For n >= 8 let k =
 *            floor(log2(n)): if n = 2^k, then a(n) = 2^(k+1)-(k+1); otherwise
 *            let r = n - 2^k and j = a(r); a(n) = j + 2^k - 1 if j <= 2^k - k
 *            - 1, a(n) = j + 2^k otherwise
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9635 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t A230599_floor_log2(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

static idx_t A230599_inv(idx_t n)
{
    static const idx_t base[7] = { 1, 3, 2, 5, 6, 4, 7 };
    if (n <= 7) return base[n - 1];

    idx_t k = A230599_floor_log2(n);
    idx_t low = (idx_t)1 << k;
    if (n == low) {
        /* 2^(k+1) - (k+1), unsigned wraparound is fine for k = 63 */
        return (low << 1) - (k + 1);
    }
    idx_t r = n - low;
    idx_t j = A230599_inv(r);
    if (j <= low - k - 1)
        return j + low - 1;
    return j + low;
}

seq_t a(idx_t n)
{
    return (seq_t)A230599_inv(n);
}
