/*
 * A094917  Contract every run of consecutive identical terms in A090822 to a
 *          single term.
 * https://oeis.org/A094917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th term of A090822 (Gijswijt's sequence) with each run
 *            of equal consecutive terms contracted to one term; computed by
 *            direct generation of A090822
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9809 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *G_arr = NULL;
static idx_t G_len = 0, G_cap = 0;
static seq_t *C_arr = NULL;
static idx_t C_len = 0, C_cap = 0;

static void G_push(seq_t v)
{
    if (G_len >= G_cap) {
        idx_t nc = G_cap ? G_cap * 2 : 64;
        G_arr = mem_resize(seq_t, G_arr, nc);
        G_cap = nc;
    }
    G_arr[G_len++] = v;
}

static void C_push(seq_t v)
{
    if (C_len >= C_cap) {
        idx_t nc = C_cap ? C_cap * 2 : 64;
        C_arr = mem_resize(seq_t, C_arr, nc);
        C_cap = nc;
    }
    C_arr[C_len++] = v;
}

static seq_t G_next_value(void)
{
    idx_t len = G_len;
    seq_t best = 1;
    seq_t last = G_arr[len - 1];
    for (idx_t bl = 1; bl <= len; ++bl) {
        if (G_arr[len - bl] != G_arr[len - 1] && bl > 1) {
            /* quick reject: last element of block must equal element bl earlier */
        }
        idx_t start = len - bl;
        seq_t count = 1;
        /* quick check using last element */
        if (len >= 2 * bl && G_arr[len - 1] != G_arr[len - 1 - bl]) continue;
        while (start >= bl) {
            bool match = true;
            for (idx_t j = bl; j-- > 0;) {
                if (G_arr[start + j] != G_arr[start - bl + j]) { match = false; break; }
            }
            if (!match) break;
            ++count;
            start -= bl;
        }
        if (count > best) best = count;
    }
    (void)last;
    return best;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (G_len == 0) {
        G_push((seq_t)1);
        G_push((seq_t)1);
    }
    while (C_len < n) {
        seq_t v = G_next_value();
        seq_t prev = G_arr[G_len - 1];
        if (v != prev) C_push(prev);
        G_push(v);
    }
    return C_arr[n - 1];
}
