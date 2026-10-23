/*
 * A190095  Number of rhombuses on a (n+1)X7 grid.
 * https://oeis.org/A190095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 149, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A190095 – Number of rhombuses on a (n+1)×7 grid
 *  For 1 ≤ n ≤ 27 the value is obtained by direct enumeration.
 *  For n > 27 the empirical recurrence holds:
 *     a(n) = 2*a(n‑1) − 2*a(n‑3) + a(n‑4)
 */
#include <stdlib.h>
#include <stddef.h>

 /* ---- Vector representation for brute‑force enumeration ---- */
typedef struct {
    int x, y, l;          /* dx, dy and squared length dx*dx+dy*dy */
} Vec;

/* Compare vectors by their squared length (used by qsort) */
static int cmp_vec(const void *a, const void *b)
{
    int la = ((const Vec *)a)->l;
    int lb = ((const Vec *)b)->l;
    return (la < lb) ? -1 : (la > lb);
}

/* Brute‑force calculation of a(n) for 1 ≤ n ≤ 27 */
static seq_t compute_a_brute(idx_t n)
{
    int n_i = (int)n;                 /* n is small (≤27) */
    size_t V = (size_t)(2 * n_i + 1) * 13 - 1;  /* (#dx)·13 – 1  (exclude 0,0) */
    Vec *vecs = (Vec *)malloc(V * sizeof(Vec));
    if (!vecs) return (seq_t)0;

    size_t pos = 0;
    for (int dx = -n_i; dx <= n_i; ++dx) {
        for (int dy = -6; dy <= 6; ++dy) {
            if (dx == 0 && dy == 0) continue;
            vecs[pos].x = dx;
            vecs[pos].y = dy;
            vecs[pos].l = dx * dx + dy * dy;
            ++pos;
        }
    }

    qsort(vecs, V, sizeof(Vec), cmp_vec);

    seq_t total = (seq_t)0;

    /* Enumerate unordered pairs of equal‑length vectors */
    size_t i = 0;
    while (i < V) {
        int len = vecs[i].l;
        size_t start = i;
        while (i < V && vecs[i].l == len) ++i;
        size_t end = i;               /* vecs[start .. end-1] have the same length */

        for (size_t a = start; a < end; ++a) {
            for (size_t b = a + 1; b < end; ++b) {
                int dx1 = vecs[a].x, dy1 = vecs[a].y;
                int dx2 = vecs[b].x, dy2 = vecs[b].y;

                /* Skip collinear pairs (cross product zero) */
                long long cross = (long long)dx1 * dy2 - (long long)dy1 * dx2;
                if (cross == 0) continue;

                /* Allowed x‑positions for the starting vertex */
                int lowx  = 0, highx = n_i;
                if (-dx1 > lowx)  lowx = -dx1;
                if (-dx2 > lowx)  lowx = -dx2;
                if (-(dx1 + dx2) > lowx) lowx = -(dx1 + dx2);
                if (n_i - dx1 < highx) highx = n_i - dx1;
                if (n_i - dx2 < highx) highx = n_i - dx2;
                if (n_i - (dx1 + dx2) < highx) highx = n_i - (dx1 + dx2);
                int countx = 0;
                if (lowx <= highx) countx = highx - lowx + 1;

                /* Allowed y‑positions for the starting vertex */
                int lowy  = 0, highy = 6;
                if (-dy1 > lowy)  lowy = -dy1;
                if (-dy2 > lowy)  lowy = -dy2;
                if (-(dy1 + dy2) > lowy) lowy = -(dy1 + dy2);
                if (6 - dy1 < highy) highy = 6 - dy1;
                if (6 - dy2 < highy) highy = 6 - dy2;
                if (6 - (dy1 + dy2) < highy) highy = 6 - (dy1 + dy2);
                int county = 0;
                if (lowy <= highy) county = highy - lowy + 1;

                total += (seq_t)countx * (seq_t)county;
            }
        }
    }

    free(vecs);
    return total / 4;   /* each rhombus counted 4 times */
}

/* -------------------------------------------------------------- */
/*  Public sequence function:  a(idx_t n)   */
/* -------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /*  Static cache of already computed terms (1‑based) */
    static seq_t cache[20001];     /* supports n up to 20000 safely      */
    static idx_t cache_max = 0;    /* largest n for which cache[n-1] is valid */

    if (n == 0) return (seq_t)0;   /* undefined for n < 1, but safe */

    /* Compute missing values up to n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        if (k <= 27) {
            cache[k-1] = compute_a_brute(k);
        } else {
            /* recurrence: a(k) = 2*a(k-1) - 2*a(k-3) + a(k-4) */
            seq_t val = ((seq_t)2 * cache[k-2])          /* 2*a(k-1)        */
                     - ((seq_t)2 * cache[k-4])          /* 2*a(k-3)        */
                     + cache[k-5];                     /* a(k-4)          */
            cache[k-1] = val;
        }
    }
    cache_max = n;
    return cache[n-1];
}
