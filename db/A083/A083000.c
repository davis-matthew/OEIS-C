/*
 * A083000  Values of x for which 9*y^2 = x^2 + 2*x*y - 2*x has integer
 *          solutions with positive y.
 * https://oeis.org/A083000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the Pell type equation (10*a(n) - 9)^2 - 10*k^2 =
 *            81, where k = sqrt(a(n)*(10*a(n)-18)). By taking the fundamental
 *            unit (721 + 228√10) of x^2 - 10*y^2 = 1, the sequence splits
 *            into three independent subsequences (indices 1,2,0 (mod 3)).
 *            Each subsequence obeys the linear recurrence...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t cache[10001 + 1] = {0};
static idx_t max_computed = 0;
static bool initialized = false;

struct cls_state {
    seq_t U;
    seq_t V;
};

static struct cls_state cls[3];   /* 0: indices 1,4,7,...  1: 2,5,8,...  2: 3,6,9,... */

static void init_states(void) {
    /* initial terms */
    cache[1] = 3;
    cache[2] = 18;
    cache[3] = 338;
    max_computed = 3;

    cls[0].U = 21;   /* 10*3 - 9 */
    cls[0].V = 6;
    cls[1].U = 171;  /* 10*18 - 9 */
    cls[1].V = 54;
    cls[2].U = 3371; /* 10*338 - 9 */
    cls[2].V = 1066;
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) {
        init_states();
    }

    if (n <= max_computed) {
        return cache[n];
    }

    for (idx_t idx = max_computed + 1; idx <= n; ++idx) {
        int cl = 0;
        switch (idx % 3) {
            case 1: cl = 0; break; /* indices 1,4,7,... */
            case 2: cl = 1; break; /* indices 2,5,8,... */
            default: cl = 2;        /* indices 0,3,6,... (e.g. 3,6,9,...) */
        }

        /* recurrence using current U,V of this class */
        seq_t newU = 721 * cls[cl].U + 2280 * cls[cl].V;
        seq_t newV = 228 * cls[cl].U + 721 * cls[cl].V;

        /* compute a(idx) = (U+9)/10 */
        cache[idx] = (newU + 9) / 10;

        /* update state for this class */
        cls[cl].U = newU;
        cls[cl].V = newV;
    }

    max_computed = n;
    return cache[n];
}
