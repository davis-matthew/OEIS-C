/*
 * A035113  Numbers != 2 (mod 4) listed in order of increasing totient
 *          function phi (A000010).
 * https://oeis.org/A035113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sequence A035113 defined by ordering numbers not congruent to 2
 *            (mod 4) by increasing φ(n). a(1) is the first such number
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* phi array used by the comparator during the current sort */
static uint64_t *phi_global = NULL;

/* Comparator for qsort: first by φ(n), then by n */
static int cmp_phi(const void *a, const void *b)
{
    const idx_t v1 = *(const idx_t *)a;
    const idx_t v2 = *(const idx_t *)b;
    uint64_t p1 = phi_global[(size_t)v1];
    uint64_t p2 = phi_global[(size_t)v2];
    if (p1 < p2) return -1;
    if (p1 > p2) return  1;
    if (v1 < v2) return -1;
    if (v1 > v2) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 0, harmless */

    static seq_t prev_term = 0;
    static bool     prev_set  = false;
    idx_t limit = 16;                      /* starting bound for the sieve */

    while (1) {
        /* --- compute φ(i) for 1 ≤ i ≤ limit --- */
        size_t arr_sz = (size_t)(limit + 1);
        uint64_t *phi = (uint64_t *)calloc(arr_sz, sizeof(uint64_t));
        if (!phi) return 0;                /* allocation failure */

        for (idx_t i = 0; i <= limit; ++i)
            phi[(size_t)i] = (uint64_t)i;

        for (idx_t i = 2; i <= limit; ++i) {
            if (phi[(size_t)i] == (uint64_t)i) { /* i is prime */
                for (idx_t j = i; j <= limit; j += i) {
                    uint64_t val = phi[(size_t)j];
                    val -= val / (uint64_t)i;
                    phi[(size_t)j] = val;
                }
            }
        }

        /* --- collect numbers not ≡ 2 (mod 4) --- */
        size_t count = 0;
        for (idx_t i = 1; i <= limit; ++i)
            if ((i & 3u) != 2u) ++count;

        idx_t *cand = (idx_t *)malloc(count * sizeof(idx_t));
        if (!cand) { free(phi); return 0; }

        size_t pos = 0;
        for (idx_t i = 1; i <= limit; ++i)
            if ((i & 3u) != 2u)
                cand[pos++] = i;   /* store using the natural integer type */

        /* --- sort candidates by φ(i), then by i --- */
        phi_global = phi;                         /* make available to the comparator */
        qsort(cand, count, sizeof(idx_t), cmp_phi);

        /* --- check whether the n-th term is available --- */
        bool stable = false;
        seq_t term = 0;
        if (n <= count) {
            term = (seq_t)cand[n-1];
            stable = prev_set && (term == prev_term);
            prev_term = term;
            prev_set  = true;
        }

        /* --- cleanup before potentially retrying with a larger limit --- */
        free(phi);
        free(cand);

        if (stable) return term;                /* the n‑th term no longer changes */

        /* --- enlarge the sieve limit for the next iteration --- */
        limit <<= 1;
        if (limit == 0) limit = ~(idx_t)0;      /* guard against unsigned overflow */
    }
}
