/*
 * A064807  Numbers which are divisible by their digital root (A010888).
 * https://oeis.org/A064807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 4196, u32 n <= 4196, u64 n <= 4196,
 *            u128 n <= 4196, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A064807 – Numbers divisible by their digital root.
 * Implementation: iterate over positive integers, keep those
 *     n such that n % digital_root(n) == 0.
 * The digital root for n>0 is (n-1)%9 + 1.  We compute it using
 * an unsigned 64‑bit reduction; for the ranges needed in the
 * tests this is sufficient.
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* static cache of computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static seq_t last_candidate = 1;  /* next integer to test */

/* helpers to manage cache */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = need * 2 + 64;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    cache = new_ptr;
    cache_cap = new_cap;
}

/* generate terms up to 'need' */
static void compute_to(idx_t need)
{
    ensure_cache(need);
    seq_t cand = last_candidate;
    for (; cache_len < need; /* increment inside loop */) {
        /* compute digital root for cand (cand > 0) */
        unsigned long long v = (unsigned long long)cand;       /* fits for tests */
        seq_t root = (seq_t)(((v - 1) % 9) + 1);
        if (cand % root == (seq_t)0) {
            cache[cache_len++] = cand;
        }
        cand++;
    }
    last_candidate = cand;
}

/* public entry point: a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined below offset, harmless */
    if (n > cache_len)
        compute_to(n);
    return cache[n-1];
}
