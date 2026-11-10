/*
 * A091407  Values of len_y(n) for A090822.
 * https://oeis.org/A091407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest block length |y| such that the word
 *            A090822(1..n-1) = x y^k with k = A090822(n) maximal (computed by
 *            direct generation of Gijswijt's sequence); a(1) = 0
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 98 terms through n = 98 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A091407_s = NULL;
static seq_t *A091407_l = NULL;
static idx_t A091407_len = 0;
static idx_t A091407_cap = 0;

static void A091407_ensure(idx_t needed)
{
    if (A091407_cap >= needed) return;
    idx_t nc = A091407_cap ? A091407_cap : 8;
    while (nc < needed) nc <<= 1;
    A091407_s = mem_resize(seq_t, A091407_s, nc);
    A091407_l = mem_resize(seq_t, A091407_l, nc);
    A091407_cap = nc;
}

static void A091407_next(void)
{
    idx_t len = A091407_len;
    A091407_ensure(len + 1);
    idx_t best = 0, bestlen = 0;
    for (idx_t bl = 1; bl <= len; ++bl) {
        idx_t start = len - bl;
        idx_t count = 1;
        while (start >= bl) {
            bool match = true;
            for (idx_t j = 0; j < bl; ++j) {
                if (A091407_s[start + j] != A091407_s[start - bl + j]) {
                    match = false;
                    break;
                }
            }
            if (!match) break;
            ++count;
            start -= bl;
        }
        if (count > best) { best = count; bestlen = bl; }
    }
    A091407_s[len] = (seq_t)best;
    A091407_l[len] = (seq_t)bestlen;
    A091407_len = len + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A091407_len < 1) {
        A091407_ensure(2);
        A091407_s[0] = (seq_t)1;
        A091407_l[0] = (seq_t)0;
        A091407_len = 1;
    }
    while (A091407_len < n)
        A091407_next();
    return A091407_l[n - 1];
}
