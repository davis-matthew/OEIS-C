/*
 * A236806  Number of (n+1)X(5+1) 0..2 arrays with the maximum plus the lower
 *          median minus the upper median minus the minimum of every 2X2
 *          subblock equal.
 * https://oeis.org/A236806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence of order 13 a(n) = 6*a(n-1)
 *            + 2037*a(n-2) - 12282*a(n-3) - 1251404*a(n-4) + 7631304*a(n-5) +
 *            222376028*a(n-6) - 1410692088*a(n-7) + 774758880*a(n-8) +
 *            9534803520*a(n-9) - 17365107200*a(n-10) - 5340748800*a(n-11) +
 *            29872128000*a(n-12) - 16293888000*a(n-13), wit...
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   mul_const, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"
#include "../utils/mul_const.h"

seq_t a(idx_t n)
{
    /* The first 13 terms, provided as decimal strings.
       They are initialized lazily the first time a(n) is called. */
    static seq_t init[14];         /* 1‑based, init[1] … init[13] */
    static bool init_done = false;

    if (!init_done) {
        static const char *leg[13] = {
            "17481", "410305", "9558409", "270610375", "6612746409",
            "203606804707", "5094225962785", "167431587932179",
            "4238424064205001", "146743496294875195",
            "3736375181313140689", "134871797219330857075",
            "3444806969589767397849"
        };
        for (idx_t i = 1; i <= 13; ++i) {
            init[i] = seq_from_str(leg[i - 1]);
        }
        init_done = true;
    }

    if (n <= 13) {
        return init[n];
    }

    /* Sliding window of the last 13 values.
       vals[0] = a(k-13), …, vals[12] = a(k-1) when computing a(k). */
    static seq_t vals[13];
    for (int i = 0; i < 13; ++i) {
        vals[i] = init[i + 1];
    }

    for (idx_t k = 14; k <= n; ++k) {
        seq_t pos = mul_const(vals[12], 6ULL)
                    + mul_const(vals[11], 2037ULL)
                    + mul_const(vals[8], 7631304ULL)
                    + mul_const(vals[7], 222376028ULL)
                    + mul_const(vals[5], 774758880ULL)
                    + mul_const(vals[4], 9534803520ULL)
                    + mul_const(vals[1], 29872128000ULL);

        seq_t neg = mul_const(vals[10], 12282ULL)
                    + mul_const(vals[9], 1251404ULL)
                    + mul_const(vals[6], 1410692088ULL)
                    + mul_const(vals[3], 17365107200ULL)
                    + mul_const(vals[2], 5340748800ULL)
                    + mul_const(vals[0], 16293888000ULL);

        seq_t new_val = pos - neg;   /* pos ≥ neg for all n ≥ 14 */

        /* shift window left, discard a(k-13), keep a(k-12) … a(k-1) */
        for (int i = 0; i < 12; ++i) {
            vals[i] = vals[i + 1];
        }
        vals[12] = new_val;
    }

    return vals[12];
}
