/*
 * A023764  Nialpdromes: digits in base 8 are in nonincreasing order.
 * https://oeis.org/A023764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th nonnegative integer whose base‑8 representation
 *            has digits that are non‑decreasing from most significant to
 *            least (OEIS A023764)
 * Range:     u8 n <= 62, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   choose_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Pick the smallest n‑th number whose base‑8 digits never increase.  */
#include <stdint.h>
#include "../utils/choose_idx.h"

/* Main sequence function.  */
seq_t a(idx_t n) {
    /* a(1) = 0 */
    if (n == 1) return (seq_t)0;

    /* Zero‑based rank among all nialpdromes.  */
    idx_t rank = n - 1;          /* n>=2 → rank>=1 */

    /* Handle single‑digit numbers (0…7).  */
    if (rank < 8) {
        return (seq_t)rank;
    }

    /*  Reduce rank by the 8 single‑digit numbers.  */
    rank -= 8U;

    /* Find the length L (number of base‑8 digits) of the desired number.  */
    int len = 2;                  /* length >=2 now */
    while (1) {
        /* Number of length‑len nialpdromes (first digit >0).  */
        idx_t cnt = choose_idx((idx_t)len + 7, (idx_t)len) - 1ULL;
        if (rank < cnt) break;
        rank -= cnt;
        ++len;
    }

    /* Construct the number digit by digit.  */
    seq_t val = 0;
    int prev = 7;                 /* maximum digit allowable for current position */

    for (int pos = len - 1; pos >= 0; --pos) {
        int remaining = pos;                  /* digits after the current one   */
        int start = (pos == len - 1) ? 1 : 0;  /* most significant digit cannot be 0 */

        for (int d = start; d <= prev; ++d) {
            /* Count of non‑increasing sequences of length `remaining`
               where each digit <= d (digits allowed: 0…d).  */
            idx_t ways = choose_idx((idx_t)remaining + (idx_t)d, (idx_t)remaining);

            if (rank < ways) {
                /* Choose digit d for this position.  */
                val = val * (seq_t)8 + (seq_t)d;
                prev = d;
                break;
            } else {
                rank -= ways;
            }
        }
    }
    return val;
}
