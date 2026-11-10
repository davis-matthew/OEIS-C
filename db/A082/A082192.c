/*
 * A082192  First row of array in A082191.
 * https://oeis.org/A082192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A082191(k(k+1)/2) for k odd, A082191(k(k-1)/2+1) for k
 *            even, k = n; computed by simulating the A082191 greedy array
 *            fill (search)
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-07)
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

#define A82_MAX_DIM 200
#define A82_USEDSIZE_INIT 1024

static seq_t *a82_grid = NULL;
static char *a82_used = NULL;
static size_t a82_usedSize = 0;

static seq_t *a82_values = NULL;
static size_t a82_valuesCap = 0;
static idx_t a82_valuesLen = 0;

static unsigned a82_currS = 3;
static unsigned a82_currPos = 0;
static bool a82_ascending = true;

static void a82_ensure_used_size(idx_t needed)
{
    if (a82_usedSize > needed) return;
    size_t ns = a82_usedSize ? a82_usedSize * 2 : A82_USEDSIZE_INIT;
    while ((idx_t)ns <= needed) ns *= 2;
    char *newp = (char *)realloc(a82_used, ns);
    if (!newp) abort();
    memset(newp + a82_usedSize, 0, ns - a82_usedSize);
    a82_used = newp;
    a82_usedSize = ns;
}

static void a82_ensure_values_capacity(size_t needed)
{
    if (a82_valuesCap >= needed) return;
    size_t nc = a82_valuesCap ? a82_valuesCap * 2 : 128;
    while (nc < needed) nc *= 2;
    seq_t *newp = mem_resize(seq_t, a82_values, nc);
    if (!newp) abort();
    a82_values = newp;
    a82_valuesCap = nc;
}

static void a82_init_storage(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    a82_grid = mem_new(seq_t, (size_t)A82_MAX_DIM * A82_MAX_DIM);
    if (!a82_grid) abort();

    a82_used = (char *)calloc(207, sizeof(char));
    a82_usedSize = 207;
    a82_used[1] = 1;

    a82_values = mem_new(seq_t, 128);
    if (!a82_values) abort();
    a82_valuesCap = 128;
    a82_valuesLen = 0;

    a82_grid[0] = 1;
    a82_values[0] = 1;
    a82_valuesLen = 1;

    a82_currS = 3;
    a82_currPos = 0;
    a82_ascending = (a82_currS % 2 == 1);
}

static void a82_next_coord(unsigned *i, unsigned *j)
{
    if (a82_ascending) {
        *i = 1 + a82_currPos;
    } else {
        *i = (a82_currS - 1) - a82_currPos;
    }
    *j = a82_currS - *i;
    a82_currPos++;
    if (a82_currPos == a82_currS - 1) {
        a82_currS++;
        a82_currPos = 0;
        a82_ascending = (a82_currS % 2 == 1);
    }
}

static seq_t A082191_a(idx_t n)
{
    if (n == 0) return 0;
    a82_init_storage();

    if (n <= a82_valuesLen) return a82_values[n - 1];

    for (idx_t idx = a82_valuesLen; idx < n; ++idx) {
        unsigned i, j;
        a82_next_coord(&i, &j);

        seq_t cand = 2;
        for (;;) {
            if ((idx_t)cand >= a82_usedSize) a82_ensure_used_size((idx_t)cand + 1);
            if (a82_used[(idx_t)cand]) { cand++; continue; }

            bool ok = true;
            for (int dx = -1; dx <= 1 && ok; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    int ni = (int)i + dx;
                    int nj = (int)j + dy;
                    if (ni < 1 || ni > A82_MAX_DIM || nj < 1 || nj > A82_MAX_DIM) continue;
                    seq_t nb = a82_grid[(size_t)(ni - 1) * A82_MAX_DIM + (nj - 1)];
                    if (nb > (seq_t)1) {
                        seq_t g = gcd(cand, nb);
                        if (g <= (seq_t)1) { ok = false; break; }
                    }
                }
            }
            if (ok) break;
            cand++;
        }

        a82_grid[(size_t)(i - 1) * A82_MAX_DIM + (j - 1)] = cand;
        a82_used[(idx_t)cand] = 1;
        a82_ensure_values_capacity(a82_valuesLen + 1);
        a82_values[a82_valuesLen++] = cand;
    }

    return a82_values[n - 1];
}

seq_t a(idx_t n)
{
    idx_t k = n;
    idx_t pos;
    if (k % 2 == 1) pos = k * (k + 1) / 2;
    else pos = k * (k - 1) / 2 + 1;
    return A082191_a(pos);
}
