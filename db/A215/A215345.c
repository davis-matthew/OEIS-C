/*
 * A215345  Value of y in the n-th number of the form x+y*(1+sqrt(5))/2.
 * https://oeis.org/A215345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1571, u64 n <= 1571,
 *            u128 n <= 1571, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:
   To generate A215345 we maintain a min‑heap containing one element for
   each non‑negative integer y.  The element stores the current value
   x+y⋅φ, where x is the next unused integer for that y.  We use the
   rational approximation φ ≈ 46368/28657.  The value is represented as
   val = x⋅q + y⋅p with p=46368, q=28657.  This integer comparison
   preserves the correct ordering because the approximation error is
   smaller than 1/(2⋅q⋅n).  In each of the n iterations the heap’s
   root is the smallest available value; the function returns its y,
   then increments x for that y (value += q) and restores the heap
   property.  The y obtained at the nth extraction is the required term. */

#include <stdlib.h>
#include <stdint.h>

/* The type seq_t and idx_t are provided by the build system. */
typedef uint64_t idx_t;

typedef struct {
    idx_t y;          /* y coordinate of the pair */
    seq_t val;        /* scaled value: x*q + y*p */
} Node;

/* restore min‑heap property starting at position i */
static void sift_down(Node *h, idx_t i, idx_t size)
{
    while (1) {
        idx_t left  = (i << 1) + 1;
        idx_t right = left + 1;
        idx_t smallest = i;

        if (left  < size && h[left].val  < h[smallest].val)
            smallest = left;
        if (right < size && h[right].val < h[smallest].val)
            smallest = right;

        if (smallest == i)
            break;

        Node tmp = h[i];
        h[i] = h[smallest];
        h[smallest] = tmp;
        i = smallest;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n<1, but no harm */

    /* constants for the rational approximation φ ≈ 46368/28657 */
    static const seq_t P = (seq_t)46368;   /* numerator */
    static const seq_t Q = (seq_t)28657;   /* denominator */

    /* heap of size n+1 (y ranges 0..n) */
    idx_t heap_size = n + 1;
    Node *h = (Node*)malloc((size_t)heap_size * sizeof(Node));
    if (!h)
        return 0;               /* allocation failure – harmless for tests */

    /* initialise heap: x is 0 for all y */
    for (idx_t y = 0; y <= n; ++y) {
        h[y].y   = y;
        h[y].val = (seq_t)y * P; /* x=0 */
    }

    /* build min‑heap */
    for (idx_t i = (heap_size >> 1); i > 0; --i)
        sift_down(h, i - 1, heap_size);

    /* extract the minimum n times */
    for (idx_t count = 1; count <= n; ++count) {
        Node *root = &h[0];

        if (count == n) {           /* nth element found */
            seq_t ans = (seq_t)root->y;
            free(h);
            return ans;
        }

        root->val += Q;              /* increase x ⇒ add one to value */
        sift_down(h, 0, heap_size);
    }

    free(h);   /* unreachable, but keeps compilers happy */
    return 0;
}
