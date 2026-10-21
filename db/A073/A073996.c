/*
 * A073996  Number of strings of length n over GF(4) with trace 0 and subtrace
 *          1.
 * https://oeis.org/A073996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Number of strings of length n over GF(4) with trace 0 and
 *            subtrace 1. Recurrence: a(n; t, s) = Σ_{k=0..3} a(n-1; t⊕k ,
 *            s⊕(k * (t⊕k)) ), where ⊕ is addition in GF(4) (XOR) and
 *            multiplication uses the GF(4) table
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t gf4_mul[4][4] = {
    {0,0,0,0},
    {0,1,2,3},
    {0,2,3,1},
    {0,3,1,2}
};

seq_t a(idx_t n) {
    static seq_t dp_prev[4][4];
    static seq_t dp_curr[4][4];

    /* Base case: length 0 string has trace 0 and subtrace 0. */
    for (uint8_t t = 0; t < 4; ++t)
        for (uint8_t s = 0; s < 4; ++s)
            dp_prev[t][s] = 0;
    dp_prev[0][0] = 1;

    for (idx_t len = 1; len <= n; ++len) {
        /* zero current table */
        for (uint8_t t = 0; t < 4; ++t)
            for (uint8_t s = 0; s < 4; ++s)
                dp_curr[t][s] = 0;

        /* recurrence */
        for (uint8_t k = 0; k < 4; ++k) {
            for (uint8_t t = 0; t < 4; ++t) {
                uint8_t tt = t ^ k;                     /* t - k */
                for (uint8_t s = 0; s < 4; ++s) {
                    uint8_t ss = s ^ gf4_mul[k][tt];   /* s - k*(t-k) */
                    dp_curr[t][s] += dp_prev[tt][ss];
                }
            }
        }

        /* swap tables for next iteration */
        for (uint8_t t = 0; t < 4; ++t)
            for (uint8_t s = 0; s < 4; ++s)
                dp_prev[t][s] = dp_curr[t][s];
    }

    /* a(n) corresponds to trace 0, subtrace 1. */
    return dp_prev[0][1];
}
