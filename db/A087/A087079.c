/*
 * A087079  Number of lunar partitions of n: number of ways of writing n as a
 *          lunar sum of distinct terms, ignoring order.
 * https://oeis.org/A087079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by inclusion–exclusion over the decimal digit
 *            positions of n. For each subset S of positions, let cnt_j be d_j
 *            if j ∈ S (digit must be < d_j) d_j + 1 otherwise (digit may be ≤
 *            d_j) where d_j is the digit of n in position j (0‑based, least
 *            significant). The number of positive inte...
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 37, u64 n <= 68, u128 n <= 168,
 *            big any n
 * Helpers:   popcount_uint32, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"   /* pow_uint(uint64_t base, idx_t exp) */
#include "../utils/popcount_uint32.h"

seq_t a(idx_t n) {
    /* Special case n == 0: the empty partition counts once. */
    if (n == 0) return 1;

    /* Extract decimal digits of n into array d[0..p-1], least significant first. */
    uint8_t d[20];           /* n <= 10^19 fits comfortably in this array */
    idx_t len = 0;
    idx_t temp = n;
    while (temp > 0) {
        d[len++] = temp % 10;
        temp /= 10;
    }

    seq_t pos = 0;   /* sum of terms with even |S|  (including the empty set)   */
    seq_t neg = 0;   /* sum of terms with odd |S|   */

    /* Iterate over all subsets of the digit positions. */
    for (uint32_t mask = 0; mask < (1U << len); ++mask) {
        seq_t prod = 1;           /* product of allowed digit counts   */
        bool zero_flag = false;

        for (uint32_t j = 0; j < len; ++j) {
            uint8_t cnt = (mask & (1U << j)) ? d[j] : (d[j] + 1);
            if (cnt == 0) {        /* no digit possible → product becomes 0 */
                zero_flag = true;
                prod = 0;
                break;
            }
            prod *= cnt;
        }

        /* Number of positive integers satisfying the constraints. */
        idx_t size = 0;
        if (!zero_flag) {
            size = (idx_t)(prod - 1);      /* subtract zero number     */
        }

        /* term = 2^{size} */
        seq_t term = pow_uint(2ULL, size);

        if ((popcount_uint32(mask) & 1) == 0) {
            pos += term;
        } else {
            neg += term;
        }
    }

    /* pos ≥ neg because the result is non‑negative. */
    return pos - neg;
}
