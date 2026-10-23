/*
 * A045115  Numbers whose base-4 representation contains exactly one 1 and one
 *          3.
 * https://oeis.org/A045115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   none; uses dynamic programming to count numbers with exactly one
 *            digit '1' and one digit '3' in base‑4, then binary searches for
 *            the n‑th such number
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Count how many integers 0 ≤ X ≤ limit have exactly one base‑4
   digit equal to 1 and exactly one base‑4 digit equal to 3. */
static idx_t count_leq(seq_t limit)
{
    /* Convert limit to base‑4 digits (most significant first). */
    seq_t tmp = limit;
    unsigned char *digits = NULL;
    size_t len = 0;
    size_t cap = 16;                         /* capacity of digits array */

    digits = (unsigned char*)malloc(cap);
    if (!digits) return 0;                    /* allocation failure */

    if (tmp == (seq_t)0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp != (seq_t)0) {
            if (len == cap) {
                cap <<= 1;
                unsigned char *newd = (unsigned char*)realloc(digits, cap);
                if (!newd) { free(digits); return 0; }
                digits = newd;
            }
            digits[len++] = (unsigned char)(tmp % (seq_t)4);
            tmp = tmp / (seq_t)4;
        }
        /* reverse to most significant first */
        for (size_t i = 0; i < len / 2; ++i) {
            unsigned char t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    size_t dim_pos = len + 1;
    size_t dim_tight = 2;
    size_t dim_c1 = 3;
    size_t dim_c3 = 3;
    size_t total = dim_pos * dim_tight * dim_c1 * dim_c3;
    idx_t *dp = (idx_t*)calloc(total, sizeof(idx_t));
    if (!dp) { free(digits); return 0; }

    /* macro for indexing into the 1‑D dp array */
    #define IDX(p, t, c1, c3) ((((p) * dim_tight + (t)) * dim_c1 + (c1)) * dim_c3 + (c3))

    dp[IDX(0, 1, 0, 0)] = 1;                 /* start state */

    for (size_t pos = 0; pos < len; ++pos) {
        for (int tight = 0; tight < 2; ++tight) {
            for (int c1 = 0; c1 < 3; ++c1) {
                for (int c3 = 0; c3 < 3; ++c3) {
                    idx_t cur = dp[IDX(pos, tight, c1, c3)];
                    if (!cur) continue;
                    int maxd = (tight ? digits[pos] : 3);
                    for (int d = 0; d <= maxd; ++d) {
                        int nc1 = c1, nc3 = c3;
                        if (d == 1) {
                            ++nc1;
                            if (nc1 > 1) continue;
                        } else if (d == 3) {
                            ++nc3;
                            if (nc3 > 1) continue;
                        }
                        int ntight = (tight && (d == maxd)) ? 1 : 0;
                        dp[IDX(pos + 1, ntight, nc1, nc3)] += cur;
                    }
                }
            }
        }
    }

    idx_t res = 0;
    for (int tight = 0; tight < 2; ++tight) {
        for (int c1 = 0; c1 < 3; ++c1) {
            for (int c3 = 0; c3 < 3; ++c3) {
                if (c1 == 1 && c3 == 1)
                    res += dp[IDX(len, tight, c1, c3)];
            }
        }
    }

    free(dp);
    free(digits);
    return res;
}

seq_t a(idx_t n)
{
    seq_t lo = (seq_t)0;
    seq_t hi = (seq_t)1;

    /* Increase hi until it contains at least n such numbers. */
    while (count_leq(hi) < n)
        hi = hi + hi;                      /* double hi */

    /* Binary search for the smallest X with count_leq(X) ≥ n. */
    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / (seq_t)2;
        idx_t cnt = count_leq(mid);
        if (cnt >= n)
            hi = mid;
        else
            lo = mid + (seq_t)1;
    }
    return lo;
}
