/*
 * A045158  Numbers whose base-5 representation contains no 0's and exactly
 *          one 1.
 * https://oeis.org/A045158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose base‑5 representation *
 *            contains no zeros and exactly one digit '1'. * * This is OEIS
 *            A045158. The algorithm enumerates numbers of * increasing base‑5
 *            length in lexicographic order. For a given * length L there are
 *            L·3^(L‑1) such numbers. Once the required * leng...
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   init_pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/init_pow3.h"

seq_t a(idx_t n)
{
    /* We only need powers of 3 up to the maximum length required
     * for the given n.  For n <= 10 000 the length never exceeds 8,
     * but we use 64 just to be safe. */
    static idx_t pow3[65] = {0};
    static bool init = false;
    if (!init) {
        init_pow3(pow3, 64);
        init = true;
    }

    /* Find the base‑5 length L that contains the nth number. */
    idx_t cumulative_before = 0;
    idx_t L;
    for (L = 1; ; ++L) {
        idx_t cnt = L * pow3[L-1];
        if (cumulative_before + cnt >= n) {
            break;
        }
        cumulative_before += cnt;
    }

    /* 0‑based offset within the numbers of length L. */
    idx_t offset0 = (n - cumulative_before) - 1;

    seq_t result = 0;
    bool placed1  = false;

    for (idx_t i = 0; i < L; ++i) {
        idx_t rem = L - i - 1;          /* remaining positions after this one */

        for (int d = 1; d <= 4; ++d) {
            idx_t cnt = 0;

            if (!placed1) {
                if (d == 1) {
                    cnt = pow3[rem];
                } else {
                    /* Must place the '1' later in one of the remaining positions. */
                    if (rem > 0) {
                        cnt = rem * pow3[rem-1];
                    }
                }
            } else {
                if (d == 1)          /* cannot use '1' again */
                    continue;
                cnt = pow3[rem];
            }

            if (cnt == 0)
                continue;

            if (offset0 < cnt) {
                result = result * (seq_t)5 + (seq_t)d;
                if (!placed1 && d == 1)
                    placed1 = true;
                break;
            } else {
                offset0 -= cnt;
            }
        }
    }

    return result;
}
