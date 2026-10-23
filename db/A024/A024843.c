/*
 * A024843  a(n) = least m such that if r and s in {1/1, 1/2, 1/3, ..., 1/n}
 *          satisfy r < s, then r < k/m < (k+3)/m < s for some integer k.
 * https://oeis.org/A024843
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest integer m for which for every i = 1 … n‑1
 *            the interval (k/m, (k+3)/m) fits strictly between 1/(i+1) and
 *            1/i for some integer k. This is equivalent to the condition:
 *            (floor(m/(i+1)) + 1 + 3) * i < m for all i
 * Range:     u8 n <= 9, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;                /* undefined for n < 2 */

    /* Lower bound coming from the strict inequality
       1/(i(i+1)) > 3/m  for i = n-1 */
    seq_t m = (seq_t)n * ((seq_t)n - 1) * 3 + 1;   /* m > 3*n*(n-1) */

    for (;;) {
        bool ok = true;
        for (idx_t i = 1; i < n; ++i) {
            seq_t div = m / ((seq_t)(i + 1));     /* floor(m/(i+1))   */
            seq_t kmin = div + 1;                 /* smallest k > m/(i+1) */
            seq_t lhs = (kmin + 3) * ((seq_t)i);   /* (k+3)*i          */
            if (lhs >= m) {                      /* condition fails   */
                ok = false;
                break;
            }
        }
        if (ok) return m;                       /* minimal m found   */
        ++m;                                     /* try next value    */
    }
}
