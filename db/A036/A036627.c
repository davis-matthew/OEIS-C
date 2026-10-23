/*
 * A036627  Number of 4-ary rooted trees with n nodes and height exactly 3.
 * https://oeis.org/A036627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = f2[n] - f1[n] where f1 counts 4‑ary rooted trees with n
 *            nodes and height ≤1, f2 counts those with height ≤2 (nodes on
 *            longest path ≤3), so a(n) counts trees whose height is exactly 3
 *            nodes
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_N 85          /* maximum nodes for height ≤2 (edges ≤2) in a 4‑ary tree */

static seq_t f1[MAX_N + 1];
static seq_t f2[MAX_N + 1];
static bool ready = false;

/* Pre‑compute f1 and f2 */
static void precompute(void)
{
    /* f1: only root or root with 0..4 leaf children */
    for (size_t i = 0; i <= MAX_N; ++i) f1[i] = 0;
    for (size_t n = 1; n <= 5; ++n) f1[n] = 1;   /* n = 1 + k, k = 0..4 */

    /* f2: multiset of up to 4 children taken from the 5 types
       types sizes are 1,2,3,4,5 (root + 0..4 leaves)             */
    for (size_t i = 0; i <= MAX_N; ++i) f2[i] = 0;
    const size_t types[5] = {1, 2, 3, 4, 5};

    /* k = 0 children (only root) */
    f2[1] += 1;                 /* 1 node */

    /* k = 1 */
    for (size_t i0 = 0; i0 < 5; ++i0) {
        size_t t = 1 + types[i0];
        if (t <= MAX_N) f2[t] += 1;
    }

    /* k = 2 */
    for (size_t i0 = 0; i0 < 5; ++i0)
        for (size_t i1 = i0; i1 < 5; ++i1) {
            size_t t = 1 + types[i0] + types[i1];
            if (t <= MAX_N) f2[t] += 1;
        }

    /* k = 3 */
    for (size_t i0 = 0; i0 < 5; ++i0)
        for (size_t i1 = i0; i1 < 5; ++i1)
            for (size_t i2 = i1; i2 < 5; ++i2) {
                size_t t = 1 + types[i0] + types[i1] + types[i2];
                if (t <= MAX_N) f2[t] += 1;
            }

    /* k = 4 */
    for (size_t i0 = 0; i0 < 5; ++i0)
        for (size_t i1 = i0; i1 < 5; ++i1)
            for (size_t i2 = i1; i2 < 5; ++i2)
                for (size_t i3 = i2; i3 < 5; ++i3) {
                    size_t t = 1 + types[i0] + types[i1] + types[i2] + types[i3];
                    if (t <= MAX_N) f2[t] += 1;
                }

    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready) precompute();

    /* n may be larger than our pre‑computed tables */
    if (n > MAX_N) return 0;

    size_t idx = (size_t)n;
    /* f2 and f1 hold exact counts; subtraction is safe because
       for n ≤ 5 we have f2[n] ≥ f1[n], for n > 5 f1[n] = 0. */
    return f2[idx] - f1[idx];
}
