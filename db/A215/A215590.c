/*
 * A215590  Value of y in the n-th number of the form x+y*(1+sqrt(2)).
 * https://oeis.org/A215590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = y where x + y·(1+√2) is the n‑th smallest value among all
 *            numbers of the form x + y·(1+√2) with x,y ≥ 0. The sequence
 *            gives the coefficient y for that n‑th number
 * Range:     u8 n <= 1064, u16 n <= 1064, u32 n <= 1064, u64 n <= 1064,
 *            u128 n <= 1064, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* A node represents one progression y = n.  m is the current integer part
   of the value: value = m + n·√2.  Initially m = n (i.e., x = 0).  After
   each extraction we increment m by 1 to advance along that progression. */
typedef struct {
    idx_t m;   /* current integer part (m ≥ n) */
    idx_t n;   /* fixed coefficient of √2 (the y value) */
} node_t;

/* Compare two values m₁ + n₁·√2 and m₂ + n₂·√2 exactly.
   Return -1 if the first is smaller, +1 otherwise. */
static int cmp_node(const node_t *a, const node_t *b)
{
    /* Use signed __int128 to avoid overflow for very large indices. */
    __int128 dm = (__int128)a->m - (__int128)b->m;   /* m₁ – m₂ */
    __int128 dn = (__int128)a->n - (__int128)b->n;   /* n₁ – n₂ */

    /* If the √2 coefficients are equal, the comparison reduces to the
       integer parts.  The values are distinct, so dm ≠ 0. */
    if (dn == 0)
        return (dm < 0) ? -1 : 1;

    /* Let d_m = dm, d_n = dn.  We compare
         d_m + d_n·√2
       The sign of this expression tells us which value is larger.
       We avoid floating point by squaring both sides.  */
    __int128 numer = -dm;                         /* -d_m */
    __int128 dn_sq  = dn * dn;                    /* d_n²  */
    __int128 numer_sq = numer * numer;            /* (-d_m)² */
    __int128 left = 2 * dn_sq;                    /* 2·d_n²   */

    /* For d_n > 0:  d_m + d_n·√2 < 0  iff left < numer_sq.
       For d_n < 0:  d_m + d_n·√2 < 0  iff left > numer_sq. */
    if (dn > 0)
        return (left < numer_sq) ? -1 : 1;         /* a < b  → -1 */
    else
        return (left > numer_sq) ? -1 : 1;         /* a < b  → -1 */
}

/* Restore the min‑heap property starting from index i. */
static void sift_down(node_t *heap, size_t size, size_t i)
{
    while (1) {
        size_t l = 2 * i + 1;
        size_t r = l + 1;
        size_t smallest = i;

        if (l < size && cmp_node(&heap[l], &heap[smallest]) < 0)
            smallest = l;
        if (r < size && cmp_node(&heap[r], &heap[smallest]) < 0)
            smallest = r;
        if (smallest == i)
            break;
        node_t tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

/* Return a(n) – the y‑value of the n‑th smallest number of the form
   x + y·(1+√2) with x,y ≥ 0.  The function works for any integer type
   seq_t supplied by the build. */
seq_t a(idx_t n)
{
    if (n == 0)      /* offset starts at 1; undefined below 1 */
        return 0;

    /* Allocate a heap with one element per possible y value up to n.
       We never need more than n distinct progression lines because
       the nth smallest element must come from a line y < n. */
    size_t heap_size = (size_t)n;
    node_t *heap = (node_t *)malloc(heap_size * sizeof(node_t));
    if (!heap)
        return 0;    /* allocation failure – return 0 as safe value */

    /* Initialise each progression: start with x = 0 → m = y. */
    for (size_t i = 0; i < heap_size; ++i) {
        heap[i].m = (idx_t)i;   /* m = y */
        heap[i].n = (idx_t)i;   /* n = y */
    }

    /* Build min‑heap. */
    for (size_t i = heap_size / 2; i > 0; --i)
        sift_down(heap, heap_size, i - 1);

    seq_t y_of_n = 0;
    for (idx_t step = 1; step <= n; ++step) {
        /* Extract the smallest value. */
        node_t *root = &heap[0];
        y_of_n = (seq_t)root->n;   /* the y of this minimal pair */
        /* Advance this progression by one (increase m). */
        root->m += 1;
        sift_down(heap, heap_size, 0);
    }

    free(heap);
    return y_of_n;
}
