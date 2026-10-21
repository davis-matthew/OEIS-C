/*
 * A062951  a(n) = H(A005117(n)) where H(n) is the half-totient function and
 *          A005117(n) are the squarefree numbers.
 * https://oeis.org/A062951
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = H(A005117(n)) = phi(A005117(n))/2 for n >= 3
 * Range:     u8 n <= 77, u16 n <= 9729, u32 n <= 9729, u64 n <= 9729,
 *            u128 n <= 9729, big any n
 * Helpers:   calc_phi, extend_cache, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/calc_phi.h"
#include "../utils/extend_cache.h"

static seq_t *a_cache   = NULL;   /* cache of a(n) for n >= 3, indexed by n-3 */
static size_t   a_cap   = 0;
static idx_t    a_len   = 0;

static seq_t *sqfree_cache = NULL;   /* cache of squarefree numbers, 1‑indexed */
static size_t   sqfree_cap   = 0;
static idx_t    sqfree_len   = 0;
static seq_t    sqfree_next = 1;

/* Return the k‑th square‑free number (k ≥ 1). */
static seq_t get_squarefree(idx_t k)
{
    if (k <= sqfree_len)
        return sqfree_cache[k-1];
    while (sqfree_len < k) {
        seq_t val = sqfree_next++;
        if (is_squarefree(val)) {
            extend_cache(&sqfree_cache, &sqfree_cap, sqfree_len + 1);
            sqfree_cache[sqfree_len++] = val;
        }
    }
    return sqfree_cache[k-1];
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;   /* value for n < 3 is irrelevant */

    idx_t idx = n - 3;                  /* index in the cached array */
    if (idx < a_len)
        return a_cache[idx];

    extend_cache(&a_cache, &a_cap, idx + 1);

    for (idx_t i = a_len; i <= idx; ++i) {
        seq_t sf   = get_squarefree(i + 3);   /* A005117(i+3) */
        seq_t phi  = calc_phi(sf);            /* φ(A005117)   */
        a_cache[i] = phi >> 1;                /* half‑totient  */
    }
    a_len = idx + 1;
    return a_cache[idx];
}
