/*
 * A123572  The Kruskal-Macaulay function K_3(n).
 * https://oeis.org/A123572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n_3,2) + n_2 + 1 where n is uniquely written as n =
 *            C(n_3,3) + C(n_2,2) + C(n_1,1) with n_3 > n_2 > n_1 >= 1. The
 *            function implements the greedy decomposition described in the
 *            OEIS entry
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool */

static inline seq_t binom2(idx_t x)
{
    /* C(x,2) = x*(x-1)/2 */
    return (seq_t)x * (seq_t)(x - 1) / 2;
}

static inline seq_t binom3(idx_t x)
{
    /* C(x,3) = x*(x-1)*(x-2)/6 */
    return (seq_t)x * (seq_t)(x - 1) * (seq_t)(x - 2) / 6;
}

seq_t a(idx_t n)
{
    /* Handle the trivial case. */
    if (n == 0)
        return 0;

    /* ---------- find n3 ---------- */
    idx_t n3 = 0;
    for (idx_t cand = 3; ; ++cand) {
        seq_t v = binom3(cand);
        if (v <= (seq_t)n) {
            n3 = cand;
        } else {
            break;
        }
    }

    seq_t rem = (seq_t)n - binom3(n3);    /* remainder after C(n3,3) */

    seq_t result = 0;
    if (n3 > 0)
        result += binom2(n3);            /* C(n3,2) */

    /* If nothing left, we are done. */
    if (rem == 0)
        return result;

    /* ---------- find n2 ---------- */
    idx_t n2 = 0;
    for (idx_t cand = 2; cand < n3; ++cand) {
        seq_t v = binom2(cand);
        if (v <= rem) {
            n2 = cand;
        } else {
            break;
        }
    }
    result += (seq_t)n2;                 /* add C(n2,1) which is n2 */
    rem -= binom2(n2);

    /* If nothing left, we are done. */
    if (rem == 0)
        return result;

    /* ---------- n1 is implicitly rem (rem >=1 and rem < n2) ---------- */
    /* Add C(rem,0) = 1 */
    result += 1;

    return result;
}
