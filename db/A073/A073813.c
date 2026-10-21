/*
 * A073813  Difference between n and largest unrelated number belonging to n,
 *          when n runs over composite numbers. For primes and for 4,
 *          unrelated set is empty.
 * https://oeis.org/A073813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = difference between the n‑th composite number c and the
 *            largest unrelated composite k < c (where 1 < gcd(k,c) < k); if
 *            no such k exists, a(n) = 0
 * Range:     u8 n <= 1528, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"

static uint64_t *composites = NULL;       /* stored composite numbers   */
static size_t    comp_cnt   = 0;          /* number of composites stored*/
static size_t    comp_cap   = 0;          /* allocated array capacity   */
static uint64_t  comp_limit = 3;          /* last tested natural number */

/* Ensure that at least 'need' composite numbers are stored. */
static void
ensure_composites(size_t need)
{
    if (comp_cnt >= need)
        return;

    if (comp_cap < need) {
        size_t new_cap = comp_cap ? comp_cap * 2 : 8;
        if (new_cap < need)
            new_cap = need;
        uint64_t *tmp = (uint64_t *)realloc(composites, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        composites = tmp;
        comp_cap   = new_cap;
    }

    while (comp_cnt < need) {
        ++comp_limit;
        if (!is_prime(comp_limit)) {
            composites[comp_cnt++] = comp_limit;
        }
    }
}

/* Return the n‑th composite number (1‑based index, n >= 1). */
static inline uint64_t
get_composite(idx_t n)
{
    ensure_composites((size_t)n);
    return composites[(size_t)(n - 1)];
}

seq_t
a(idx_t n)
{
    if (n == 0)          /* undefined below offset 1; return 0 for safety */
        return (seq_t)0;

    uint64_t m = get_composite(n);      /* n‑th composite */

    /* Search downward for the largest unrelated composite k. */
    for (uint64_t k = m - 1; ; --k) {
        if (k < 4)                       /* no composite smaller than 4 */
            break;
        if (!is_prime(k)) {              /* k must be composite */
            seq_t g = gcd((seq_t)k, (seq_t)m);
            if (g > (seq_t)1 && (seq_t)k > g)
                return (seq_t)(m - k);
        }
        if (k == 4)                      /* avoid unsigned wrap‑around */
            break;
    }
    return (seq_t)0;                     /* unrelated set empty */
}
