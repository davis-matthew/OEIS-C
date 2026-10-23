/*
 * A139248  Triangle read by rows: row n lists the proper divisors of n-th
 *          even superperfect number A061652(n).
 * https://oeis.org/A139248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2^k, where k is the zero‑based index of n in the row of
 *            the n‑th even superperfect number
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"

static const idx_t mersenne_exps[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 13ULL, 17ULL, 19ULL, 31ULL, 61ULL, 89ULL,
    107ULL, 127ULL, 521ULL, 607ULL, 1279ULL, 2203ULL, 2281ULL, 3217ULL,
    4253ULL, 4423ULL, 9689ULL, 9941ULL, 10691ULL, 11057ULL, 12271ULL,
    13007ULL, 20259ULL, 22877ULL, 23233ULL, 23497ULL, 23701ULL, 24133ULL,
    24943ULL, 26161ULL, 27647ULL, 28093ULL, 31059ULL, 31691ULL, 33757ULL,
    37073ULL, 37157ULL, 42287ULL, 60191ULL, 73867ULL, 80633ULL, 81391ULL,
    81917ULL, 82433ULL
};
static const size_t exps_cnt = sizeof(mersenne_exps) / sizeof(mersenne_exps[0]);

seq_t a(idx_t n) {
    idx_t cumulative = 0;
    for (size_t i = 0; i < exps_cnt; ++i) {
        idx_t row_terms = mersenne_exps[i] - 1;
        if (n <= cumulative + row_terms) {
            idx_t offset = n - cumulative - 1;  /* 0‑based position in the row */
            return pow2(offset);
        }
        cumulative += row_terms;
    }
    /* n is beyond the range covered by the precomputed list. */
    return 0;
}
