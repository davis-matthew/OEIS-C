/*
 * A045241  Numbers whose base-5 representation contains exactly two 1's and
 *          one 3.
 * https://oeis.org/A045241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer whose base‑5 representation contains
 *            exactly two 1's and one 3
 * Range:     u8 n <= 21, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   search_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/search_len.h"

/* integer power for 64‑bit results – sufficient for the tiny exponents needed */
static uint64_t pow_uint64(uint64_t base, unsigned exp)
{
    uint64_t res = 1;
    while (exp--) res *= base;
    return res;
}

/* number of admissible numbers of a given length L (L ≥ 3) */
static uint64_t count_length(idx_t L)
{
    if (L < 3) return 0;

    uint64_t pow3 = pow_uint64(3, (unsigned)(L - 3));       /* 3^(L‑3) */

    /* Case A – leading digit 1: choose one more 1 and one 3 in the rest */
    uint64_t countA = (uint64_t)(L - 1) * (L - 2) * pow3;

    /* Case B – leading digit 3: choose two 1’s in the rest */
    uint64_t combB = (uint64_t)(L - 1) * (L - 2) / 2;        /* C(L‑1,2) */
    uint64_t countB = combB * pow3;

    /* Case C – leading digit 2 or 4: choose two 1’s and one 3 in the rest */
    uint64_t countC = 0;
    if (L >= 4) {
        uint64_t pow3C = pow_uint64(3, (unsigned)(L - 4));   /* 3^(L‑4) */
        countC = 2 * combB * (L - 3) * pow3C;                /* 2 for 2 or 4 */
    }

    return countA + countB + countC;
}

/* public interface required by the build system */
seq_t a(idx_t n)
{
    /* locate the length that contains the nth term */
    uint64_t cumulative = 0;
    idx_t L = 3;
    while (1) {
        uint64_t cnt = count_length(L);
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
        ++L;
    }
    idx_t offset = (idx_t)(n - cumulative);   /* 1‑based position within length L */

    /* generate the offset‑th number of this length */
    seq_t result = 0;
    search_len(L, 0, 2, 1, 0, &offset, &result);
    return result;
}
