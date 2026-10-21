/*
 * A099958  (1/2)*number of distinct angular positions under which an observer
 *          positioned at the center of an edge of a square lattice can see
 *          the (2n)X(2n-1) points symmetrically surrounding his position.
 * https://oeis.org/A099958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..2n-1} phi(2k) (sum of Euler totients of even
 *            numbers up to 4n-2). The function recomputes φ up to the
 *            required maximum on demand and keeps a prefix‑sum array of the
 *            even totients so that each call runs in O(1) after at most one
 *            sieve
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build system. */

static seq_t *even_prefix = NULL;
static idx_t prefix_limit = 0;

/* Sieve φ for all numbers up to new_limit and build the even‑prefix sum. */
static void recompute_even_prefix(idx_t new_limit)
{
    /* Allocate and initialise φ array. */
    uint64_t *phi = (uint64_t *)malloc((size_t)(new_limit + 1) * sizeof(uint64_t));
    if (!phi) return;                               /* allocation failed */
    for (idx_t i = 0; i <= new_limit; ++i) phi[i] = i;

    /* Classic Euler sieve for φ. */
    for (idx_t p = 2; p <= new_limit; ++p) {
        if (phi[p] == p) {                           /* p is prime */
            for (idx_t j = p; j <= new_limit; j += p)
                phi[j] -= phi[j] / p;
        }
    }

    /* Resize prefix array. */
    seq_t *new_pref = (seq_t *)realloc(even_prefix,
                                       (size_t)(new_limit + 1) * sizeof(seq_t));
    if (!new_pref) {                                 /* allocation failed */
        free(phi);
        return;
    }
    even_prefix = new_pref;

    /* Compute prefix sums of φ over even indices. */
    even_prefix[0] = 0;
    for (idx_t i = 1; i <= new_limit; ++i) {
        even_prefix[i] = even_prefix[i - 1];
        if ((i & 1) == 0)                            /* i even */
            even_prefix[i] += (seq_t)phi[i];
    }

    prefix_limit = new_limit;
    free(phi);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                            /* offset 1, but safe guard */
    idx_t max_even = 4 * n - 2;                      /* largest even ≤ 4n−2 */
    if (max_even > prefix_limit) {
        recompute_even_prefix(max_even);
    }
    return even_prefix[max_even];
}
