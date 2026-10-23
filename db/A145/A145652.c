/*
 * A145652  a(1)=1. a(n) = the largest integer such that the finite sequence
 *          (a(n-1),a(n-2),...a(n-a(n))) occurs somewhere as a subsequence in
 *          the finite sequence (a(1),a(2),...,a(n-1)).
 * https://oeis.org/A145652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. For n > 1, a(n) is the largest integer k (1 ≤ k ≤ n‑1)
 *            such that the reversed suffix (a(n-1), a(n-2), …, a(n-k)) occurs
 *            as a contiguous subsequence somewhere in (a(1), a(2), …,
 *            a(n-1)). Once n ≥ 14, the sequence becomes 12‑periodic: a(n+12)
 *            = a(n) for all n ≥ 14
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t seq[26];
static bool init = false;

static seq_t next_value(idx_t m) /* compute a(m) for m ≥ 2 */
{
    /* m is already ≥ 2.  seq[0..m-2] contains a(1)…a(m‑1).  */
    for (idx_t k = m - 1; k >= 1; --k) {
        bool found = false;
        /* start indices in [0, m‑k-1]  */
        for (idx_t start = 0; start <= (m - 1) - k; ++start) {
            bool ok = true;
            for (idx_t j = 0; j < k; ++j) {
                /* compare a[start + j] with a[(m-2)-j]  */
                if (seq[start + j] != seq[(m - 2) - j]) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                found = true;
                break;
            }
        }
        if (found)
            return (seq_t)k;
    }
    /* should never happen; guarantees at least k = 1 */
    return 0;
}

seq_t a(idx_t n)
{
    if (!init) {
        seq[0] = (seq_t)1;              /* a(1) = 1 */
        for (idx_t m = 2; m <= 25; ++m) /* compute a(2)…a(25) */
            seq[m - 1] = next_value(m);
        init = true;
    }

    if (n <= 25)
        return seq[n - 1];

    /* 12‑periodic behaviour for n ≥ 26 (i.e., n ≥ 14 + 12) */
    idx_t offset = (n - 14) % 12;
    return seq[14 + offset - 1];
}
