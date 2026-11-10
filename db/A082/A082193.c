/*
 * A082193  First column of array in A082191.
 * https://oeis.org/A082193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A082191(k), where k = (n-1)*n/2 + 1 if n+1 is even, else
 *            n*(n+1)/2 (position of cell (n,1) in the fill order of the array
 *            in A082191); computed by simulating the array fill (search)
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
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

#define A082191_MAX_DIM 200
#define A082191_USEDSIZE_INIT 1024

static seq_t *A082191_grid        = NULL;
static char *A082191_used         = NULL;
static size_t A082191_usedSize    = 0;

static seq_t *A082191_values      = NULL;
static size_t A082191_valuesCap   = 0;
static idx_t  A082191_valuesLen   = 0;

static unsigned A082191_currS     = 3;
static unsigned A082191_currPos   = 0;
static bool    A082191_ascending  = true;

static void A082191_ensure_used_size(idx_t needed)
{
    if (A082191_usedSize > needed) return;
    size_t ns = A082191_usedSize ? A082191_usedSize * 2 : A082191_USEDSIZE_INIT;
    while ((idx_t)ns <= needed) ns *= 2;
    char *newp = (char *)realloc(A082191_used, ns);
    if (!newp) abort();
    memset(newp + A082191_usedSize, 0, ns - A082191_usedSize);
    A082191_used = newp;
    A082191_usedSize = ns;
}

static void A082191_ensure_values_capacity(size_t needed)
{
    if (A082191_valuesCap >= needed) return;
    size_t nc = A082191_valuesCap ? A082191_valuesCap * 2 : 128;
    while (nc < needed) nc *= 2;
    seq_t *newp = mem_resize(seq_t, A082191_values, nc);
    if (!newp) abort();
    A082191_values = newp;
    A082191_valuesCap = nc;
}

static void A082191_init_storage(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    A082191_grid = mem_new(seq_t, (size_t)A082191_MAX_DIM * A082191_MAX_DIM);
    if (!A082191_grid) abort();

    A082191_used = (char *)calloc(207, sizeof(char));
    A082191_usedSize = 207;
    A082191_used[1] = 1;

    A082191_values = mem_new(seq_t, 128);
    if (!A082191_values) abort();
    A082191_valuesCap = 128;
    A082191_valuesLen = 0;

    A082191_grid[0] = 1;
    A082191_values[0] = 1;
    A082191_valuesLen = 1;

    A082191_currS = 3;
    A082191_currPos = 0;
    A082191_ascending = (A082191_currS % 2 == 1);
}

static void A082191_next_coord(unsigned *i, unsigned *j)
{
    if (A082191_ascending) {
        *i = 1 + A082191_currPos;
    } else {
        *i = (A082191_currS - 1) - A082191_currPos;
    }
    *j = A082191_currS - *i;
    A082191_currPos++;
    if (A082191_currPos == A082191_currS - 1) {
        A082191_currS++;
        A082191_currPos = 0;
        A082191_ascending = (A082191_currS % 2 == 1);
    }
}

static seq_t A082191_a(idx_t n)
{
    if (n == 0) return 0;
    A082191_init_storage();

    if (n <= A082191_valuesLen) return A082191_values[n - 1];

    for (idx_t idx = A082191_valuesLen; idx < n; ++idx) {
        unsigned i, j;
        A082191_next_coord(&i, &j);

        seq_t cand = 2;
        for (;;) {
            if ((idx_t)cand >= A082191_usedSize) A082191_ensure_used_size((idx_t)cand + 1);
            if (A082191_used[(idx_t)cand]) { cand++; continue; }

            bool ok = true;
            for (int dx = -1; dx <= 1 && ok; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    int ni = (int)i + dx;
                    int nj = (int)j + dy;
                    if (ni < 1 || ni > A082191_MAX_DIM || nj < 1 || nj > A082191_MAX_DIM) continue;
                    seq_t nb = A082191_grid[(size_t)(ni - 1) * A082191_MAX_DIM + (nj - 1)];
                    if (nb > (seq_t)1) {
                        seq_t g = gcd(cand, nb);
                        if (g <= (seq_t)1) { ok = false; break; }
                    }
                }
            }
            if (ok) break;
            cand++;
        }

        A082191_grid[(size_t)(i - 1) * A082191_MAX_DIM + (j - 1)] = cand;
        A082191_used[(idx_t)cand] = 1;
        A082191_ensure_values_capacity(A082191_valuesLen + 1);
        A082191_values[A082191_valuesLen++] = cand;
    }

    return A082191_values[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t s = n + 1;
    idx_t k;
    if (s % 2 == 0) k = (s - 2) * (s - 1) / 2 + 1;
    else k = (s - 1) * s / 2;
    return A082191_a(k);
}
