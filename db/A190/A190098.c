/*
 * A190098  T(n,k)=Number of rhombuses on a (n+1)X(k+1) grid.
 * https://oeis.org/A190098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 875, u32 n <= 5064, u64 n <= 5064,
 *            u128 n <= 5064, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A190098: Number of rhombuses on a (r+1)×(c+1) integer lattice grid.
    The sequence a(n) is obtained by reading the two‑dimensional array
    T(r,c) in anti‑diagonal order (r+c constant, r decreasing).  */
#include <stddef.h>
#include <stdlib.h>

/* Helper structure for a lattice vector */
typedef struct {
    int dx;
    int dy;
    int len;   /* squared length dx*dx + dy*dy */
} vec_t;

/* Compare function for sorting vectors by squared length, then coordinates */
static int cmp_vec(const void *a, const void *b)
{
    const vec_t *va = (const vec_t *)a;
    const vec_t *vb = (const vec_t *)b;
    if (va->len < vb->len) return -1;
    if (va->len > vb->len) return 1;
    if (va->dx  < vb->dx)  return -1;
    if (va->dx  > vb->dx)  return  1;
    if (va->dy  < vb->dy)  return -1;
    if (va->dy  > vb->dy)  return  1;
    return 0;
}

/* Test whether two non‑zero vectors are collinear (one is a multiple of the other) */
static int collinear(const vec_t *a, const vec_t *b)
{
    return a->dx * b->dy == a->dy * b->dx;
}

/* Maximal r or c that we may encounter for n ≤ 10000.
   s ≈ √(2n), so 201 is safe. */
#define MAXRC 201

static seq_t cache[MAXRC][MAXRC];
static int  computed[MAXRC][MAXRC];

/* Count rhombuses on a (r+1)×(c+1) lattice grid.  r,c ≥ 1.  */
static seq_t count_rhombus(idx_t r, idx_t c)
{
    if (computed[r][c])
        return cache[r][c];

    /* Build vector list */
    size_t cnt = (size_t)(2*r + 1) * (size_t)(2*c + 1) - 1;
    vec_t *vecs = (vec_t *)malloc(cnt * sizeof(vec_t));
    if (!vecs)
        return 0;   /* allocation failure – should not happen for the small sizes here */

    size_t idx = 0;
    for (int dx = -(int)r; dx <= (int)r; ++dx) {
        for (int dy = -(int)c; dy <= (int)c; ++dy) {
            if (dx == 0 && dy == 0) continue;
            vecs[idx].dx  = dx;
            vecs[idx].dy  = dy;
            vecs[idx].len = dx*dx + dy*dy;
            ++idx;
        }
    }

    /* Sort vectors by squared length to group equal‑length vectors together */
    qsort(vecs, cnt, sizeof(vec_t), cmp_vec);

    /* Enumerate unordered pairs within each length group */
    seq_t total = 0;
    size_t i = 0;
    while (i < cnt) {
        size_t j = i + 1;
        while (j < cnt && vecs[j].len == vecs[i].len) ++j;   /* [i, j) is a group */

        for (size_t a = i; a < j; ++a) {
            const vec_t *va = &vecs[a];
            for (size_t b = a + 1; b < j; ++b) {
                const vec_t *vb = &vecs[b];
                if (collinear(va, vb)) continue;          /* skip collinear vectors */

                /* Coordinates of the four vertices: (0,0), va, vb, va+vb */
                int minx = 0, maxx = 0;
                int miny = 0, maxy = 0;

                if (va->dx < minx) minx = va->dx;
                if (va->dx > maxx) maxx = va->dx;
                if (vb->dx < minx) minx = vb->dx;
                if (vb->dx > maxx) maxx = vb->dx;
                int vdxc = va->dx + vb->dx;
                if (vdxc < minx) minx = vdxc;
                if (vdxc > maxx) maxx = vdxc;

                if (va->dy < miny) miny = va->dy;
                if (va->dy > maxy) maxy = va->dy;
                if (vb->dy < miny) miny = vb->dy;
                if (vb->dy > maxy) maxy = vb->dy;
                int vdyc = va->dy + vb->dy;
                if (vdyc < miny) miny = vdyc;
                if (vdyc > maxy) maxy = vdyc;

                int width  = maxx - minx;
                int height = maxy - miny;

                if (width > (int)r || height > (int)c) continue;

                seq_t positions = (seq_t)(r + 1 - width) * (seq_t)(c + 1 - height);
                total += positions;
            }
        }
        i = j;
    }

    free(vecs);
    total /= 4;               /* each rhombus counted once per corner */

    computed[r][c] = 1;
    cache[r][c] = total;
    return total;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find sum s such that (s-1)*s/2 >= n and (s-2)*(s-1)/2 < n */
    idx_t s = 2;
    while ((s - 1) * s / 2 < n)
        ++s;
    idx_t prev = (s - 2) * (s - 1) / 2;
    idx_t offset = n - prev;          /* 1 ≤ offset ≤ s-1 */
    idx_t r = s - offset;             /* 1 ≤ r ≤ s-1 */
    idx_t c = offset;                 /* 1 ≤ c ≤ s-1 */

    /* Ensure the cache arrays are large enough */
    if (r >= MAXRC || c >= MAXRC)
        return 0;  /* safety check – not expected for permissible n */

    return count_rhombus(r, c);
}
