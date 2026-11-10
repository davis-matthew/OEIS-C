/*
 * A211025  Triangle read by rows: T(n,k) is the total sum of parts in the
 *          last section of the modular table of partitions of n after k-th
 *          stage.
 * https://oeis.org/A211025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = Sum_{j=1..k} A135010(n,j); row n has
 *            A006128(n)-A006128(n-1) terms: p(n-1) ones, then the parts of
 *            the 1-free partitions of n in lexicographic order. Computed by
 *            walking the rows and enumerating partitions of n with parts >= 2
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

#define A211025_MAXR 400

static uint64_t A211025_P[A211025_MAXR];
static int A211025_init = 0;

static void A211025_setup(void)
{
    if (A211025_init) return;
    for (int i = 0; i < A211025_MAXR; ++i) A211025_P[i] = 0;
    A211025_P[0] = 1;
    for (int k = 1; k < A211025_MAXR; ++k)
        for (int i = k; i < A211025_MAXR; ++i)
            A211025_P[i] += A211025_P[i - k];
    A211025_init = 1;
}

/* number of partitions of m with no part 1 */
static uint64_t A211025_q(int m)
{
    if (m < 0) return 0;
    if (m == 0) return 1;
    return A211025_P[m] - A211025_P[m - 1];
}

/* total number of parts over all partitions of r with no part 1 */
static uint64_t A211025_parts_no1(int r)
{
    uint64_t s = 0;
    for (int k = 2; k <= r; ++k)
        for (int j = 1; j * k <= r; ++j)
            s += A211025_q(r - j * k);
    return s;
}

static bool A211025_rec(int sum, int minp, idx_t *rem, seq_t *acc,
                        int *parts, int depth)
{
    if (sum == 0) {
        for (int i = 0; i < depth; ++i) {
            *acc += (seq_t)parts[i];
            if (--(*rem) == 0) return true;
        }
        return false;
    }
    for (int part = minp; part <= sum; ++part) {
        int left = sum - part;
        if (left != 0 && left < part) continue;
        parts[depth] = part;
        if (A211025_rec(left, part, rem, acc, parts, depth + 1))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    A211025_setup();
    idx_t m = n;
    int r = 1;
    for (;; ++r) {
        idx_t len = (idx_t)(A211025_P[r - 1] + A211025_parts_no1(r));
        if (m <= len) break;
        m -= len;
    }
    idx_t ones = (idx_t)A211025_P[r - 1];
    if (m <= ones) return (seq_t)m;
    seq_t acc = (seq_t)ones;
    idx_t rem = m - ones;
    int parts[512];
    A211025_rec(r, 2, &rem, &acc, parts, 0);
    return acc;
}
