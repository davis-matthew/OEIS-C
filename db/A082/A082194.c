/*
 * A082194  Main diagonal of array in A082191.
 * https://oeis.org/A082194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A(n,n), the main diagonal of the array in A082191;
 *            computed by simulating the greedy fill (smallest unused integer
 *            sharing a factor >1 with every filled neighbour, in order of i+j
 *            with alternating direction)
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-07)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

#define A082194_DIM 256

static seq_t *A082194_grid = NULL;
static seq_t *A082194_diag = NULL;
static char *A082194_used = NULL;
static size_t A082194_usedSize = 0;
static unsigned A082194_s = 3;
static unsigned A082194_p = 0;
static idx_t A082194_have = 0;

static void A082194_ensure_used(size_t needed)
{
    if (A082194_usedSize > needed) return;
    size_t ns = A082194_usedSize ? A082194_usedSize : 1024;
    while (ns <= needed) ns *= 2;
    char *np = (char *)realloc(A082194_used, ns);
    if (!np) abort();
    memset(np + A082194_usedSize, 0, ns - A082194_usedSize);
    A082194_used = np;
    A082194_usedSize = ns;
}

static void A082194_init(void)
{
    if (A082194_grid) return;
    A082194_grid = mem_new(seq_t, (size_t)A082194_DIM * A082194_DIM);
    A082194_diag = mem_new(seq_t, A082194_DIM);
    A082194_ensure_used(2048);
    A082194_used[1] = 1;
    A082194_grid[0] = 1;
    A082194_diag[1] = 1;
    A082194_have = 1;
}

seq_t a(idx_t n)
{
    if (n == 0 || n > 120) return 0;
    A082194_init();
    while (A082194_have < n) {
        unsigned s = A082194_s;
        unsigned i, j;
        if (s % 2 == 1) i = 1 + A082194_p;
        else i = (s - 1) - A082194_p;
        j = s - i;
        A082194_p++;
        if (A082194_p == s - 1) { A082194_s++; A082194_p = 0; }

        seq_t cand = 2;
        for (;;) {
            size_t c = (size_t)cand;
            A082194_ensure_used(c + 1);
            if (A082194_used[c]) { cand++; continue; }
            bool ok = true;
            for (int dx = -1; dx <= 1 && ok; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    int ni = (int)i + dx, nj = (int)j + dy;
                    if (ni < 1 || ni > A082194_DIM || nj < 1 || nj > A082194_DIM) continue;
                    seq_t nb = A082194_grid[(size_t)(ni - 1) * A082194_DIM + (size_t)(nj - 1)];
                    if (nb > (seq_t)1) {
                        if (gcd(cand, nb) <= (seq_t)1) { ok = false; break; }
                    }
                }
            }
            if (ok) break;
            cand++;
        }
        A082194_grid[(size_t)(i - 1) * A082194_DIM + (size_t)(j - 1)] = cand;
        A082194_used[(size_t)cand] = 1;
        if (i == j) {
            A082194_diag[i] = cand;
            A082194_have = (idx_t)i;
        }
    }
    return A082194_diag[n];
}
