/*
 * A107576  a(n)=perimeter of n-th triangle listed at A107572.
 * https://oeis.org/A107576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Function : a(n) = the n-th perimeter of an integer-scalene triangle
 *  sorted by increasing perimeter, then by side length a, then by side b.
 *  The routine enumerates all triangles with side lengths a<b<c≤300,
 *  stores their perimeters, sorts them accordingly, and returns the
 *  requested term.  It uses a single pass construction and keeps the
 *  list in static memory for subsequent calls.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Triangle record used only during sorting. */
typedef struct {
    seq_t per;     /* perimeter */
    idx_t a, b;    /* sides a<b, the third side is per - a - b */
} Triangle;

/* Comparison function for qsort: first by perimeter, then by a, then by b. */
static int triangle_cmp(const void *p1, const void *p2)
{
    const Triangle *t1 = (const Triangle *)p1;
    const Triangle *t2 = (const Triangle *)p2;

    if (t1->per < t2->per) return -1;
    if (t1->per > t2->per) return 1;

    if (t1->a < t2->a) return -1;
    if (t1->a > t2->a) return 1;

    if (t1->b < t2->b) return -1;
    if (t1->b > t2->b) return 1;

    return 0;
}

seq_t a(idx_t n)
{
    /* Static data prepared on first call. */
    static seq_t *perims = NULL;   /* array of perimeters in order */
    static idx_t  count   = 0;     /* number of triangles generated */
    static bool   ready   = false;

    if (!ready)
    {
        /* Generate all scalene triangles with sides 2 ≤ a < b < c ≤ MAX_SIDE.
         * MAX_SIDE = 300 gives more than 2 million triangles, far exceeding
         * the 10 000 terms typically queried, while keeping memory usage
         * modest (~30 MB). */
        const idx_t MAX_SIDE = 300;

        /* Temporary array to store triangle records before sorting. */
        idx_t cap = 1024;
        Triangle *tris = (Triangle *)malloc(cap * sizeof(Triangle));
        count = 0;

        for (idx_t a = 2; a <= MAX_SIDE; ++a)
        {
            for (idx_t b = a + 1; b <= MAX_SIDE; ++b)
            {
                /* The smallest possible c is b + 1.
                 * If a + b <= b + 1 no triangle is possible for this (a,b). */
                if (a + b <= b + 1) continue;

                idx_t sum = a + b;
                for (idx_t c = b + 1; c <= MAX_SIDE && sum > c; ++c)
                {
                    /* Ensure dynamic array has space. */
                    if (count == cap)
                    {
                        cap <<= 1;
                        tris = (Triangle *)realloc(tris, cap * sizeof(Triangle));
                    }

                    /* Store triangle record. */
                    tris[count].per = (seq_t)a + (seq_t)b + (seq_t)c;
                    tris[count].a   = a;
                    tris[count].b   = b;
                    ++count;
                }
            }
        }

        /* Sort the triangle records. */
        qsort(tris, count, sizeof(Triangle), triangle_cmp);

        /* Extract the sorted perimeters into a compact array. */
        perims = (seq_t *)malloc(count * sizeof(seq_t));
        for (idx_t i = 0; i < count; ++i)
            perims[i] = tris[i].per;

        /* Free temporary storage. */
        free(tris);

        ready = true;
    }

    /* The sequence is 1‑based.  If n is out of range, return 0. */
    if (n == 0 || n > count)
        return (seq_t)0;

    return perims[n - 1];
}
