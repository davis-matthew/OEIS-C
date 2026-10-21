/*
 * A082191  In a square array, positive integers are entered as follows. Start
 *          with a(1, 1)=1. Then enter numbers in this order: a(1, 2), a(2,
 *          1), a(3, 1), a(2, 2), a(1, 3), a(1, 4), a(2, 3), a(3, 2), a(4, 1),
 *          a(5, 1), a(4, 2), ... such that every entry is the smallest not
 *          yet used and such that every entry...
 * https://oeis.org/A082191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A082191 – fill a square array with the smallest unused
   integer that shares a non‑trivial divisor with every already‑filled
   neighbour (in all eight directions).  The array is traversed by
   increasing sums (i + j) with
         odd sums : i increasing (1…s−1)
         even sums: i decreasing (s−1…1)
   This function returns the n‑th term (1‑based).  The implementation
   generates coordinates on demand and keeps a cache of values; it works
   for arbitrary seq_t sizes, using only integer operations. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

/* ------------------------------------------------------------------ configuration */
#define MAX_DIM 200   /* sufficient for n up to ~10000 */
#define USEDSIZE_INIT 1024

/* ------------------------------------------------------------------ static storage */
static seq_t *grid        = NULL;      /*[ (i-1)*MAX_DIM + (j-1) ]*/
static char *used        = NULL;      /* 0 = unused, 1 = used (index == number) */
static size_t usedSize   = 0;

static seq_t *values     = NULL;      /* cached sequence values a(1)…a(n) */
static size_t valuesCap  = 0;
static idx_t  valuesLen  = 0;

static unsigned currS    = 3;         /* current sum i+j */
static unsigned currPos  = 0;         /* position within current sum */
static bool    ascending = true;      /* true if i increasing for currS */

/* ------------------------------------------------------------------ helpers */

/* enlarge the 'used' array so that index 'needed' is valid */
static void ensure_used_size(idx_t needed)
{
    if (usedSize > needed) return;
    size_t ns = usedSize ? usedSize * 2 : USEDSIZE_INIT;
    while ((idx_t)ns <= needed) ns *= 2;
    char *newp = (char *)realloc(used, ns);
    if (!newp) abort();
    memset(newp + usedSize, 0, ns - usedSize);
    used = newp;
    usedSize = ns;
}

/* enlarge the values cache */
static void ensure_values_capacity(size_t needed)
{
    if (valuesCap >= needed) return;
    size_t nc = valuesCap ? valuesCap * 2 : 128;
    while (nc < needed) nc *= 2;
    seq_t *newp = (seq_t *)realloc(values, nc * sizeof(seq_t));
    if (!newp) abort();
    values = newp;
    valuesCap = nc;
}

/* initialise static data, first time the function is called */
static void init_storage(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* grid */
    grid = (seq_t *)calloc((size_t)MAX_DIM * MAX_DIM, sizeof(seq_t));
    if (!grid) abort();

    /* used numbers */
    used = (char *)calloc(207, sizeof(char)); /* initial size for 1..206 */
    usedSize = 207;
    used[1] = 1;                          /* number 1 is used at (1,1) */

    /* sequence values cache */
    values = (seq_t *)malloc(sizeof(seq_t) * 128);
    if (!values) abort();
    valuesCap = 128;
    valuesLen = 0;

    /* first term a(1)=1 placed at (1,1) */
    grid[0] = 1;
    values[0] = 1;
    valuesLen = 1;

    /* generator state for subsequent coordinates */
    currS = 3;          /* next sum i+j = 3 (i.e. (1,2),(2,1)) */
    currPos = 0;
    ascending = (currS % 2 == 1);
}

/* return next coordinate (i,j) in 1‑based indexing, modify generator state */
static void next_coord(unsigned *i, unsigned *j)
{
    if (ascending) {
        *i = 1 + currPos;
    } else {
        *i = (currS - 1) - currPos;
    }
    *j = currS - *i;
    currPos++;
    if (currPos == currS - 1) {     /* move to next sum */
        currS++;
        currPos = 0;
        ascending = (currS % 2 == 1);
    }
}

/* ------------------------------------------------------------------ main function */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, safety net */
    init_storage();

    if (n <= valuesLen) return values[n - 1];

    /* compute missing entries up to n */
    for (idx_t idx = valuesLen; idx < n; ++idx) {
        unsigned i, j;
        next_coord(&i, &j);

        /* search for the smallest unused value satisfying neighbour gcds */
        seq_t cand = 2;
        for (;;) {
            /* enlarge used array if necessary */
            if ((idx_t)cand >= usedSize) ensure_used_size((idx_t)cand + 1);
            if (used[(idx_t)cand]) { cand++; continue; }

            bool ok = true;
            for (int dx = -1; dx <= 1 && ok; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    int ni = (int)i + dx;
                    int nj = (int)j + dy;
                    if (ni < 1 || ni > MAX_DIM || nj < 1 || nj > MAX_DIM) continue;
                    seq_t nb = grid[(size_t)(ni - 1) * MAX_DIM + (nj - 1)];
                    if (nb > (seq_t)1) {
                        seq_t g = gcd(cand, nb);
                        if (g <= (seq_t)1) { ok = false; break; }
                    }
                }
            }
            if (ok) break;
            cand++;
        }

        /* assign function */
        grid[(size_t)(i - 1) * MAX_DIM + (j - 1)] = cand;
        used[(idx_t)cand] = 1;
        ensure_values_capacity(valuesLen + 1);
        values[valuesLen++] = cand;
    }

    return values[n - 1];
}
