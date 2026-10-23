/*
 * A107838  Leftmost digit of a(n) represents the number of odd digits in
 *          a(n); rightmost digit of a(n) represents the number of even digits
 *          in a(n).
 * https://oeis.org/A107838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the nth smallest positive integer whose first digit
 *            equals its count of odd digits and whose last digit equals its
 *            count of even digits
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom64, pow5_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom64.h"
#include "../utils/pow5_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but harmless */

    idx_t rem = n;                 /* remaining index to find (1‑based) */

    for (idx_t L = 3; ; ++L) {     /* length of the number (L≥3) */
        for (idx_t O = 1; O <= 9 && O <= L; ++O) {
            idx_t E = L - O;
            if (E > 9) continue;   /* last digit must be 0–9 */

            idx_t leading_parity = O & 1;
            idx_t trailing_parity = E & 1;
            long long reqOdd_ll = (long long)O - (leading_parity + trailing_parity);
            if (reqOdd_ll < 0) continue;
            idx_t reqOdd = (idx_t)reqOdd_ll;

            idx_t m = L - 2;        /* interior positions */
            if (reqOdd > m) continue;

            idx_t group_cnt = binom64(m, reqOdd) * pow5_u64(m);

            if (rem > group_cnt) {
                rem -= group_cnt;
                continue;
            }

            /* the desired term lies in this group */
            idx_t offset = rem - 1;   /* 0‑based index within the group */
            seq_t value = (seq_t)O;   /* leading digit */

            idx_t odd_left = reqOdd;
            idx_t positions = m;

            for (idx_t pos = 0; pos < positions; ++pos) {
                for (int d = 0; d <= 9; ++d) {
                    idx_t p = d & 1;                     /* parity of chosen digit */
                    if (odd_left < p) continue;

                    idx_t remaining = positions - pos - 1;
                    if (odd_left - p > remaining) continue;

                    idx_t ways = binom64(remaining, odd_left - p)
                                 * pow5_u64(remaining);

                    if (ways <= offset) {
                        offset -= ways;
                        continue;
                    }

                    /* this digit is the correct one */
                    value = value * (seq_t)10 + (seq_t)d;
                    odd_left -= p;
                    break;
                }
            }

            /* append the last digit */
            value = value * (seq_t)10 + (seq_t)E;
            return value;
        }
    }
}
