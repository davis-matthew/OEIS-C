/*
 * A045282  Numbers whose base-5 representation contains exactly one 2 and one
 *          4.
 * https://oeis.org/A045282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration: numbers whose base‑5 representation contains exactly one 2 and one 4 */
#include <stdbool.h>

static seq_t g_result;
static idx_t g_target;
static idx_t g_count;
static bool g_done;

/* Recursive generation of base‑5 numbers of a fixed total length. */
static void gen_rec(idx_t pos, idx_t len, seq_t cur, bool used2, bool used4)
{
    if (g_done) return;
    if (pos == len) {
        if (used2 && used4) {
            ++g_count;
            if (g_count == g_target) {
                g_result = cur;
                g_done = true;
            }
        }
        return;
    }
    for (int d = 0; d < 5; ++d) {
        if (pos == 0 && d == 0) continue;           /* no leading zero */
        bool n2 = used2, n4 = used4;
        if (d == 2) {
            if (used2) continue;
            n2 = true;
        } else if (d == 4) {
            if (used4) continue;
            n4 = true;
        }
        seq_t new_val = cur * (seq_t)5 + d;
        gen_rec(pos + 1, len, new_val, n2, n4);
        if (g_done) return;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined offset 0 */
    g_target = n;
    g_count  = 0;
    g_done   = false;
    for (idx_t len = 2; !g_done; ++len) {
        gen_rec(0, len, (seq_t)0, false, false);
    }
    return g_result;
}
