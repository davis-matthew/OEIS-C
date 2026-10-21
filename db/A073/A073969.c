/*
 * A073969  Number of strings of length n over Z_5 with trace 1 and subtrace
 *          3.
 * https://oeis.org/A073969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the number of strings of length n over Z_5 with trace 1
 *            and subtrace 3. Uses dynamic programming over trace (t) and
 *            subtrace (s), each modulo 5
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* state arrays indexed by trace and subtrace modulo 5 */
    seq_t prev[5][5] = {0};
    seq_t next[5][5] = {0};

    /* initial state: empty string has trace 0 and subtrace 0 */
    prev[0][0] = 1;

    /* iterate over lengths 1 .. n */
    for (idx_t len = 1; len <= n; ++len) {
        /* reset next */
        for (int t = 0; t < 5; ++t)
            for (int s = 0; s < 5; ++s)
                next[t][s] = 0;

        for (int t = 0; t < 5; ++t) {
            for (int s = 0; s < 5; ++s) {
                seq_t val = prev[t][s];
                if (val == 0) continue;

                /* append character v = 0..4 */
                /* v = 0 */
                next[t][s] += val; /* trace unchanged, subtrace unchanged */

                /* v = 1 */
                {
                    int nt = (t + 1) % 5;
                    int ns = (s + t + 1) % 5;
                    next[nt][ns] += val;
                }
                /* v = 2 */
                {
                    int nt = (t + 2) % 5;
                    int ns = (s + 2 * t + 4) % 5;
                    next[nt][ns] += val;
                }
                /* v = 3 */
                {
                    int nt = (t + 3) % 5;
                    int ns = (s + 3 * t + 4) % 5;
                    next[nt][ns] += val;
                }
                /* v = 4 */
                {
                    int nt = (t + 4) % 5;
                    int ns = (s + 4 * t + 1) % 5;
                    next[nt][ns] += val;
                }
            }
        }

        /* swap prev and next for next iteration */
        for (int t = 0; t < 5; ++t)
            for (int s = 0; s < 5; ++s)
                prev[t][s] = next[t][s];
    }

    /* after n characters, return count for trace=1, subtrace=3 */
    return prev[1][3];
}
