/*
 * A073986  Number of strings of length n over Z_6 with trace 2 and subtrace
 *          3.
 * https://oeis.org/A073986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the number of length-n strings over Z₆ with trace 2 and
 *            subtrace 3. The recurrence a(n; t, s) = Σₓ∈Z₆ a(n‑1; (t+x) mod6,
 *            (s + x·t) mod6) (mod 6 arithmetic for trace and subtrace) is
 *            used. The initial counts for length 1 are a(1; t, s) = 1 if s==0
 *            and t==a (a∈0..5), otherwise 0. The desire...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* arrays to hold counts for current and next length */
    static seq_t curr[6][6];
    static seq_t next[6][6];
    /* initialise for length 1: trace = symbol, subtrace = 0 */
    for (int t = 0; t < 6; ++t) {
        for (int s = 0; s < 6; ++s) {
            curr[t][s] = 0;
        }
        curr[t][0] = 1;        /* one string consisting of symbol t */
    }

    /* if n==1, return value for trace 2 and subtrace 3 */
    if (n <= 1) {
        return curr[2][3];
    }

    /* iterate lengths 2 .. n */
    for (idx_t len = 2; len <= n; ++len) {
        /* initialise next table to zero */
        for (int t = 0; t < 6; ++t) {
            for (int s = 0; s < 6; ++s) {
                next[t][s] = 0;
            }
        }
        /* propagate counts */
        for (int t = 0; t < 6; ++t) {
            for (int s = 0; s < 6; ++s) {
                seq_t val = curr[t][s];
                if (!val) continue;
                for (int x = 0; x < 6; ++x) {
                    int t2 = t + x;
                    if (t2 >= 6) t2 -= 6;
                    int add = (x * t) % 6;
                    int s2 = s + add;
                    if (s2 >= 6) s2 -= 6;
                    next[t2][s2] += val;
                }
            }
        }
        /* swap curr and next */
        for (int t = 0; t < 6; ++t) {
            for (int s = 0; s < 6; ++s) {
                curr[t][s] = next[t][s];
            }
        }
    }

    /* return the required value */
    return curr[2][3];
}
