/*
 * A032936  Numbers whose set of base-16 digits is {1,2}.
 * https://oeis.org/A032936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=0}^{k-1} (1 + ((n - (2^k-2)-1) >> i & 1)) * 16^i *
 *            where k is the smallest integer such that 2^(k+1)-2 ≥ n
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 510, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Determine the block length k */
    idx_t k = 1;
    while ((1ULL << (k + 1)) - 2ULL < n)
        ++k;

    /* Numbers with fewer than k hex‑digits: 2^k-2 */
    idx_t total_prev = (1ULL << k) - 2ULL;

    /* Offset within the k‑digit block (0‑based) */
    idx_t offset = n - total_prev - 1ULL;

    seq_t result = 0;
    seq_t pow16   = 1;      /* 16^0 */
    for (idx_t i = 0; i < k; ++i)
    {
        seq_t bit = (offset >> i) & 1ULL;      /* bit i of offset */
        seq_t digit = 1 + bit;                 /* 1 or 2 */
        result += digit * pow16;
        pow16 *= 16;                           /* next power of 16 */
    }
    return result;
}
