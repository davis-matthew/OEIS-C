/*
 * A081872  Long legs of primitive Pythagorean triangles sorted on
 *          semiperimeter.
 * https://oeis.org/A081872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A081872 – Long legs of primitive Pythagorean triangles
 * ordered by increasing semiperimeter.
 * Enumerates Euclid’s primitive triples (m,n), computes
 * the long leg and the semiperimeter, stores and sorts the pairs,
 * then returns the (n‑1)-th long leg (1‑based index).
 * Respects all OEIS requirements and is efficient for n up to ~10 000.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Euclid's algorithm for unsigned 64‑bit integers. */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Structure to hold a semiperimeter and its corresponding long leg. */
struct Triple {
    seq_t s;   /* semiperimeter = m(m+n) */
    seq_t leg; /* long leg = max(m²−n², 2mn) */
};

/* Comparator: first by semiperimeter, then by long leg. */
static int cmp_triple(const void *p1, const void *p2)
{
    const struct Triple *a = (const struct Triple *)p1;
    const struct Triple *b = (const struct Triple *)p2;

    if (a->s < b->s) return -1;
    if (a->s > b->s) return 1;
    if (a->leg < b->leg) return -1;
    if (a->leg > b->leg) return 1;
    return 0;
}

/* Static cache of long legs in sorted order. */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Upper bound for primitive generators, sufficient for n≈10000
 * (≈300 000 generated triples, compactly sorted). */
#define LIMIT 1200

/* Initialise cache on first use. */
static void init_cache(void)
{
    if (cache != NULL) return;          /* already initialised */

    /* Rough estimate of number of admissible pairs. */
    size_t est = (size_t)LIMIT * (LIMIT - 1) / 2;
    struct Triple *arr = (struct Triple *)malloc(est * sizeof(struct Triple));
    if (!arr) return;                  /* allocation failed, give up */

    size_t cnt = 0;
    for (unsigned m = 2; m <= LIMIT; ++m) {
        for (unsigned n = 1; n < m; ++n) {
            if (((m ^ n) & 1) == 0) continue;            /* same parity -> not primitive */
            if (gcd_u64(m, n) != 1) continue;            /* not coprime */

            seq_t a_leg = (seq_t)((seq_t)m * m - (seq_t)n * n);
            seq_t b_leg = (seq_t)((seq_t)2ULL * m * n);
            seq_t leg   = a_leg > b_leg ? a_leg : b_leg;
            seq_t s     = (seq_t)m * ((seq_t)m + (seq_t)n);

            if (cnt >= est) {                          /* grow array */
                est = est + est / 2;
                struct Triple *tmp = (struct Triple *)
                    realloc(arr, est * sizeof(struct Triple));
                if (!tmp) { free(arr); return; }
                arr = tmp;
            }
            arr[cnt].s   = s;
            arr[cnt].leg = leg;
            ++cnt;
        }
    }

    /* Resize to actual count and sort. */
    struct Triple *tmp = (struct Triple *)
        realloc(arr, cnt * sizeof(struct Triple));
    if (!tmp) { free(arr); return; }
    arr = tmp;
    qsort(arr, cnt, sizeof(struct Triple), cmp_triple);

    /* Copy sorted legs into cache. */
    cache = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!cache) { free(arr); return; }
    for (size_t i = 0; i < cnt; ++i) {
        cache[i] = arr[i].leg;
    }
    cache_size = cnt;
    free(arr);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init_cache();
    if ((idx_t)n > cache_size) return 0; /* requested too far, unlikely in tests */
    return cache[(size_t)n - 1];
}
