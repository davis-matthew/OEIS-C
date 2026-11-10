/*
 * A091408  Values of len_x(n) for A090822.
 * https://oeis.org/A091408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of x in the decomposition a(1..n-1) = x y^k of
 *            Gijswijt's sequence A090822 (k maximal, y the shortest such
 *            block); a(1)=0. Computed by direct search
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static unsigned int *A091408_g = NULL;   /* Gijswijt terms, 0-based */
static size_t *A091408_lx = NULL;        /* len_x for position i+1 */
static size_t A091408_len = 0;
static size_t A091408_cap = 0;

static void A091408_step(void)
{
    size_t len = A091408_len;
    if (len + 1 > A091408_cap) {
        size_t nc = A091408_cap ? A091408_cap * 2 : 64;
        A091408_g = (unsigned int *)realloc(A091408_g, nc * sizeof(unsigned int));
        A091408_lx = (size_t *)realloc(A091408_lx, nc * sizeof(size_t));
        A091408_cap = nc;
    }
    if (len == 0) {
        A091408_g[0] = 1;
        A091408_lx[0] = 0;
        A091408_len = 1;
        return;
    }
    size_t best = 0, bestlx = 0;
    for (size_t bl = 1; bl <= len; ++bl) {
        size_t start = len - bl;
        size_t count = 1;
        while (start >= bl) {
            bool match = true;
            for (size_t j = 0; j < bl; ++j) {
                if (A091408_g[start + j] != A091408_g[start - bl + j]) { match = false; break; }
            }
            if (!match) break;
            ++count;
            start -= bl;
        }
        if (count > best) {
            best = count;
            bestlx = len - count * bl;
        }
    }
    A091408_g[len] = (unsigned int)best;
    A091408_lx[len] = bestlx;
    A091408_len = len + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A091408_len < (size_t)n)
        A091408_step();
    return (seq_t)A091408_lx[(size_t)n - 1];
}
