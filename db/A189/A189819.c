/*
 * A189819  Partial sums of A189816.
 * https://oeis.org/A189819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = floor((n+1)/3) - a(floor((n+1)/3)) + floor(n/3)
 *            - a(floor(n/3)) (n>=1) This implements recurrence with
 *            memoisation
 * Range:     u8 n <= 677, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* array of computed values     */
static unsigned char *comp = NULL;   /* array of flags (0=unknown)   */
static size_t cap = 0;               /* current capacity of arrays   */

static void
ensure_cache_size(idx_t n)
{
    /* enlarge cache to hold indices 0..n (inclusive) */
    if (n < cap && comp && cache) return;

    size_t new_cap = cap ? cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t  *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    unsigned char *new_comp =
        (unsigned char*)malloc(new_cap * sizeof(unsigned char));
    if (!new_cache || !new_comp) { free(new_cache); free(new_comp); exit(1); }

    if (cache) {
        memcpy(new_cache, cache, cap * sizeof(seq_t));
        memcpy(new_comp, comp, cap * sizeof(unsigned char));
    }
    memset(new_cache + cap, 0, (new_cap - cap) * sizeof(seq_t));
    memset(new_comp + cap, 0, (new_cap - cap) * sizeof(unsigned char));

    free(cache); free(comp);
    cache = new_cache; comp = new_comp; cap = new_cap;

    /* base case a(0) = 0 */
    comp[0] = 1;
    cache[0] = 0;
}

static seq_t
a_recursive(idx_t n)
{
    ensure_cache_size(n);
    if (comp[n]) return cache[n];

    idx_t q1 = (n + 1) / 3;
    idx_t q2 = n / 3;
    seq_t res =
        (seq_t)q1 - a_recursive(q1) + (seq_t)q2 - a_recursive(q2);

    cache[n] = res;
    comp[n] = 1;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the public API but needed internally */
    return a_recursive(n);
}
