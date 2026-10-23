/*
 * A180825  Number of distinct solutions of sum{i=1..2}(x(2i-1)*x(2i)) = 1
 *          (mod n), with x() only in 2..n-2.
 * https://oeis.org/A180825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered solutions of x1*x2 + x3*x4 ≡ 1 (mod
 *            n) with 2 ≤ xi ≤ n-2
 * Range:     u8 n <= 16, u16 n <= 84, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n ≤ 4 the allowed range is empty or has a single value,
       hence no solution can exist. */
    if (n <= 4)
        return 0;

    /* Count unordered pairs (a,b) with product modulo n. */
    seq_t *cnt = (seq_t*)calloc((size_t)n, sizeof(seq_t));
    if (!cnt)
        return 0;   /* allocation failure */

    for (idx_t a = 2; a <= n - 2; ++a) {
        for (idx_t b = a; b <= n - 2; ++b) {
            idx_t r = (idx_t)(((seq_t)a * (seq_t)b) % (seq_t)n);
            ++cnt[r];
        }
    }

    seq_t ans = 0;
    for (idx_t r = 0; r < n; ++r) {
        seq_t cr = cnt[r];
        if (cr == 0)
            continue;

        idx_t s = (n + 1 - r) % n;          /* s ≡ 1 – r (mod n) */

        if (r < s) {
            ans += cr * cnt[s];
        } else if (r == s) {                /* r + r ≡ 1 (mod n) */
            if ((r + r) % n == 1) {
                ans += cr * (cr + 1) / 2;
            }
        }
    }

    free(cnt);
    return ans;
}
