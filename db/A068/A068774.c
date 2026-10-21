/*
 * A068774  Number of strings over Z_4 of length n with trace 0 and subtrace
 *          2.
 * https://oeis.org/A068774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n; t, s) = a(n‑1; t, s) + a(n‑1; t+3, s+3t+1) + a(n‑1; t+2,
 *            s+2t) + a(n‑1; t+1, s+t+1) where t, s ∈ Z/4. Initial values:
 *            a(1; t, 0) = 1 for t=0..3, all other a(1; t, s)=0. The required
 *            sequence is a(n) = a(n; 0, 2). The offset is 1 (a(1)=0)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n == 1 the answer is simply 0, but the loop handles it as well. */
    seq_t prev[16] = {0};
    seq_t next[16];

    /* Initialize for strings of length 1. */
    for (idx_t t = 0; t < 4; ++t)
        prev[t * 4 + 0] = (seq_t)1;

    if (n == 1)
        return prev[0 * 4 + 2];  /* a(1;0,2) */

    for (idx_t len = 2; len <= n; ++len) {
        /* compute next from prev */
        for (int i = 0; i < 16; ++i)
            next[i] = 0;

        for (idx_t t = 0; t < 4; ++t) {
            for (idx_t s = 0; s < 4; ++s) {
                seq_t val = prev[t * 4 + s];          /* a(n-1; t, s) */

                idx_t t1 = (t + 3) & 3;                      /* t+3 mod 4 */
                idx_t s1 = (s + 3 * t + 1) & 3;              /* s+3t+1 mod 4 */
                val += prev[t1 * 4 + s1];

                idx_t t2 = (t + 2) & 3;                      /* t+2 mod 4 */
                idx_t s2 = (s + 2 * t) & 3;                  /* s+2t mod 4 */
                val += prev[t2 * 4 + s2];

                idx_t t3 = (t + 1) & 3;                      /* t+1 mod 4 */
                idx_t s3 = (s + t + 1) & 3;                 /* s+t+1 mod 4 */
                val += prev[t3 * 4 + s3];

                next[t * 4 + s] = val;
            }
        }

        /* swap prev and next for the next iteration */
        for (int i = 0; i < 16; ++i) {
            seq_t v = prev[i];
            prev[i] = next[i];
            next[i] = v;
        }
    }

    return prev[0 * 4 + 2];   /* a(n;0,2) */
}
