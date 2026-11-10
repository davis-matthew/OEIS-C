/*
 * A211030  Sum of all parts in the structure of the modular table of
 *          partitions of A135010 after n-th stage.
 * https://oeis.org/A211030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Sum_{k=1..n} A135010(k); rows of A135010 are generated
 *            directly (p(r-1) ones, then partitions of r with parts >= 2 in
 *            lexicographic order) and summed until n terms are used
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static idx_t g_rem;
static seq_t g_total;
static int g_parts[512];
static idx_t g_p[320];
static bool g_init = false;

static bool A211030_gen(int sum, int min_part, int depth)
{
    if (sum == 0) {
        for (int i = 0; i < depth; ++i) {
            g_total += (seq_t)g_parts[i];
            if (--g_rem == 0) return true;
        }
        return false;
    }
    for (int part = min_part; part <= sum; ++part) {
        int left = sum - part;
        if (left != 0 && left < part) continue;
        g_parts[depth] = part;
        if (A211030_gen(left, part, depth + 1)) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (!g_init) {
        g_init = true;
        g_p[0] = 1;
        for (int i = 1; i < 320; ++i) g_p[i] = 0;
        for (int k = 1; k < 320; ++k)
            for (int i = k; i < 320; ++i)
                g_p[i] += g_p[i - k];
    }
    g_rem = n;
    g_total = 0;
    for (int r = 1; r < 320; ++r) {
        idx_t ones = g_p[r - 1];
        if (g_rem <= ones) {
            g_total += (seq_t)g_rem;
            return g_total;
        }
        g_total += (seq_t)ones;
        g_rem -= ones;
        if (A211030_gen(r, 2, 0)) return g_total;
    }
    return g_total;
}
