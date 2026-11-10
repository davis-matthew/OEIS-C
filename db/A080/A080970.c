/*
 * A080970  A014486-indices of the trees whose interior zigzag-tree (Stanley's
 *          c) is not branch-reduced (in the sense defined by Donaghey).
 * https://oeis.org/A080970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A080300(A080971(n)). Computed by a search: list the Dyck
 *            words of A014486 in order and keep the index of each word whose
 *            binary tree (word = 1 L 0 R) is not branch-reduced. A tree is
 *            not branch-reduced when some node has exactly one child, on the
 *            same side as the edge from its own parent
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 68 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define A080970_DIR_NONE  0
#define A080970_DIR_LEFT  1
#define A080970_DIR_RIGHT 2

static idx_t *A080970_found = NULL;
static size_t A080970_cnt = 0;
static size_t A080970_cap = 0;
static idx_t A080970_next_index = 1;   /* index 0 = empty tree (reduced) */
static int A080970_next_k = 1;

static void A080970_push(idx_t v)
{
    if (A080970_cnt == A080970_cap) {
        size_t nc = A080970_cap ? A080970_cap * 2 : 64;
        idx_t *p = (idx_t *)realloc(A080970_found, nc * sizeof(idx_t));
        if (!p) return;
        A080970_found = p;
        A080970_cap = nc;
    }
    A080970_found[A080970_cnt++] = v;
}

/* returns true if subtree in bits[lo..hi) has the bad pattern */
static bool A080970_bad(const int *bits, int lo, int hi, int dir)
{
    int h, m, Llo, Lhi, Rlo, Rhi;
    bool lempty, rempty;
    if (lo >= hi) return false;
    h = 0;
    m = lo;
    for (;;) {
        h += bits[m] ? 1 : -1;
        if (h == 0) break;
        m++;
    }
    Llo = lo + 1; Lhi = m;
    Rlo = m + 1;  Rhi = hi;
    lempty = (Llo >= Lhi);
    rempty = (Rlo >= Rhi);
    if (!lempty && rempty && dir == A080970_DIR_LEFT) return true;
    if (lempty && !rempty && dir == A080970_DIR_RIGHT) return true;
    if (A080970_bad(bits, Llo, Lhi, A080970_DIR_LEFT)) return true;
    return A080970_bad(bits, Rlo, Rhi, A080970_DIR_RIGHT);
}

static void A080970_gen(int *buf, int L, int pos, int ones, int zeros)
{
    if (pos == L) {
        if (A080970_bad(buf, 0, L, A080970_DIR_NONE))
            A080970_push(A080970_next_index);
        A080970_next_index++;
        return;
    }
    if (zeros < ones) {
        buf[pos] = 0;
        A080970_gen(buf, L, pos + 1, ones, zeros + 1);
    }
    if (ones < L / 2) {
        buf[pos] = 1;
        A080970_gen(buf, L, pos + 1, ones + 1, zeros);
    }
}

seq_t a(idx_t n)
{
    int buf[128];
    while ((idx_t)A080970_cnt <= n) {
        int k = A080970_next_k;
        int L = 2 * k;
        if (L > 128) break;
        buf[0] = 1;
        A080970_gen(buf, L, 1, 1, 0);
        A080970_next_k++;
    }
    if ((idx_t)A080970_cnt <= n) return (seq_t)0;
    return (seq_t)A080970_found[n];
}
