/*
 * A045251  Numbers whose base-5 representation contains exactly one 1 and no
 *          4's.
 * https://oeis.org/A045251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth smallest positive integer whose base‑5 representation
 *            * contains exactly one digit 1 and no digit 4
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* pre‑compute powers of 3 up to a safe maximum length */
#define MAX_POW3 200
static seq_t pow3[MAX_POW3];

static inline seq_t ways(idx_t rem, bool need1)
{
    if (!need1)
        return pow3[rem];          /* 3^rem completions */
    if (rem == 0)
        return 0;                  /* no place for the required 1 */
    return (seq_t)rem * pow3[rem - 1];  /* choose position for the 1 */
}

seq_t a(idx_t n)
{
    if (n == 0)          /* below offset, undefined */
        return 0;

    seq_t cumulative = 0;
    idx_t   L = 0;
    seq_t   p = 1;       /* will hold 3^(L-2) before each iteration for L>=2 */
    seq_t   countL = 0;

    /* Find the length L of the n‑th number */
    while (1) {
        ++L;
        if (L == 1) {
            countL = 1;
        } else {
            /* p currently equals 3^(L-2) */
            countL = p * (seq_t)(2 * L + 1);
        }
        cumulative += countL;
        if (cumulative >= n)
            break;
        if (L >= 2)
            p *= 3;        /* prepare p = 3^(L-1) for next iteration */
    }

    seq_t prevCumulative = cumulative - countL;
    seq_t offset = (seq_t)n - prevCumulative - 1; /* zero‑based index within length L */

    /* Pre‑compute 3^i for i = 0 .. L */
    pow3[0] = 1;
    for (idx_t i = 1; i <= L; ++i)
        pow3[i] = pow3[i - 1] * 3;

    seq_t ans   = 0;
    bool   need1 = true;          /* still need to place the single '1' */

    for (idx_t pos = L; pos-- > 0; ) {
        idx_t rem_after = pos;
        int start = (pos == L - 1) ? 1 : 0;   /* leading digit cannot be 0 */

        for (int cand = start; cand <= 3; ++cand) {
            if (cand == 1 && !need1)          /* second '1' cannot be placed */
                continue;

            bool need1_after = need1 && (cand != 1);
            seq_t cnt = ways(rem_after, need1_after);

            if (offset < cnt) {
                ans = ans * 5 + (seq_t)cand;
                need1 = need1_after;
                break;
            } else {
                offset -= cnt;
            }
        }
    }

    return ans;
}
