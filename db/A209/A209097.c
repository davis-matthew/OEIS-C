/*
 * A209097  Number of n X 5 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A209097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 121*a(n-1) -2777*a(n-2) +23322*a(n-3) -88925*a(n-4)
 *            +181863*a(n-5) -277613*a(n-6) +499822*a(n-7) -787540*a(n-8)
 *            +679392*a(n-9) -232128*a(n-10) with initial values a(1)=41,
 *            a(2)=3552, a(3)=329064, a(4)=30836932, a(5)=2896732704,
 *            a(6)=272236743760, a(7)=25586970618660,
 *            a(8)=2404896311723064,...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

static seq_t precompute_base(idx_t *n) {
    static bool initialized = false;
    static seq_t base[12];
    if (!initialized) {
        static const char *strs[12] = {
            "41",
            "3552",
            "329064",
            "30836932",
            "2896732704",
            "272236743760",
            "25586970618660",
            "2404896311723064",
            "226034468180738520",
            "21244822678014930356",
            "1996786217098139769792",
            "187676559324985292352416"
        };
        for (idx_t i = 0; i < 12; ++i) {
            base[i] = seq_from_str(strs[i]);
        }
        initialized = true;
    }
    return base[*n - 1];
}

seq_t a(idx_t n) {
    /* Handle base values directly. */
    if (n <= 12) {
        return precompute_base(&n);
    }

    /* Prepare a window of the last ten terms: a(k-1) ... a(k-10) */
    seq_t window[10];
    /* window[0] = a(k-1), window[9] = a(k-10)   */
    for (idx_t i = 0; i < 10; ++i) {
        /* a(12-i) */
        idx_t idx = 12 - i;
        if (idx <= 12) {
            window[i] = precompute_base(&idx);
        }
    }

    seq_t result = 0;
    for (idx_t k = 13; k <= n; ++k) {
        result  = 121 * window[0];
        result -= 2777 * window[1];
        result += 23322 * window[2];
        result -= 88925 * window[3];
        result += 181863 * window[4];
        result -= 277613 * window[5];
        result += 499822 * window[6];
        result -= 787540 * window[7];
        result += 679392 * window[8];
        result -= 232128 * window[9];

        /* shift window for next iteration */
        for (idx_t j = 9; j > 0; --j) {
            window[j] = window[j-1];
        }
        window[0] = result;
    }

    return result;
}
