/*
 * A183988  Ranks of (even i)+j*r, when all i+j*r are ranked, r=golden ratio,
 *          i>=0, j>=0.
 * https://oeis.org/A183988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the rank of the n‑th pair (i,j) having even i in the
 *            increasing order of i + j*φ, with φ the golden ratio
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper struct for the min‑heap */
struct Node {
    idx_t i;   /* i component (non‑negative) */
    idx_t j;   /* j component (non‑negative) */
};

/* Compare two nodes: return 1 if a < b, else 0 */
static int node_lt(const struct Node *a, const struct Node *b)
{
    /* use signed 64‑bit for the differences */
    long long da_j = (long long)a->j - (long long)b->j;
    long long da_s = (long long)a->i - (long long)b->i;
    long long t   = 2 * da_s + da_j;              /* 2*(i1-i2)+(j1-j2) */

    if (da_j == 0)            /* same j */
        return t < 0;          /* compare 2*(i1-i2) */
    if (da_j > 0) {
        if (t >= 0) return 0; /* a >= b */
        /* need t*t > 5*d*d  (t < 0) */
        long long d = da_j;
        return (t * t) > 5 * d * d;
    } else {                  /* da_j < 0 */
        long long d = -da_j;  /* positive */
        if (t <= 0)
            return 1;         /* a < b */
        /* need t*t < 5*d*d  (t > 0) */
        return (t * t) < 5 * d * d;
    }
}

/* heap operations ------------------------------------------------------- */

static void sift_up(struct Node *heap, size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (!node_lt(&heap[idx], &heap[parent]))
            break;
        struct Node tmp = heap[idx];
        heap[idx] = heap[parent];
        heap[parent] = tmp;
        idx = parent;
    }
}

static void sift_down(struct Node *heap, size_t idx, size_t size)
{
    while (1) {
        size_t left  = (idx << 1) + 1;
        if (left >= size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < size && node_lt(&heap[right], &heap[left]))
            smallest = right;
        if (!node_lt(&heap[smallest], &heap[idx]))
            break;
        struct Node tmp = heap[idx];
        heap[idx] = heap[smallest];
        heap[smallest] = tmp;
        idx = smallest;
    }
}

/* ----------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n==0, return 0 */

    /* a rough upper bound for the number of pairs needed */
    idx_t max_j = 3 * n + 10;          /* enough for n up to 10 000 */
    size_t heap_size = (size_t)(max_j + 1);

    /* allocate heap */
    struct Node *heap = (struct Node *)malloc(heap_size * sizeof(struct Node));
    if (!heap) return 0; /* allocation failed */

    /* initialise heap with all j lines (i=0) */
    for (size_t j = 0; j < heap_size; ++j) {
        heap[j].i = 0;
        heap[j].j = j;
    }

    /* build heap */
    for (size_t i = heap_size / 2; i-- > 0; )
        sift_down(heap, i, heap_size);

    /* generate until we find the n‑th even‑i pair */
    idx_t rank_counter = 0;          /* current rank (starting from 1) */
    size_t even_count = 0;           /* number of even‑i pairs seen */

    while (1) {
        /* pop minimum */
        struct Node u = heap[0];
        --heap_size;
        if (heap_size) {
            heap[0] = heap[heap_size];
            sift_down(heap, 0, heap_size);
        }

        ++rank_counter;
        if ((u.i & 1ULL) == 0) {           /* i is even */
            ++even_count;
            if (even_count == n) {
                free(heap);
                return (seq_t)rank_counter;
            }
        }

        /* push next pair on the same j line */
        struct Node v;
        v.i = u.i + 1;
        v.j = u.j;
        heap[heap_size] = v;
        sift_up(heap, heap_size);
        ++heap_size;
    }
}
