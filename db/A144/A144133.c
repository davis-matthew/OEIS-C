/*
 * A144133  a(n) = Gegenbauer polynomial C_n^2(3).
 * https://oeis.org/A144133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((14*n+13)*p(2*n+1) + 6*(n+1)*p(2*n)) / 32, where p(k) is
 *            the Pell sequence with p(0)=1, p(1)=2 (p(k)=2*p(k-1)+p(k-2) for
 *            k>=2)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *p_cache   = NULL;   /* Pell numbers */
static size_t p_cache_size = 0;   /* number of elements stored */
static size_t p_cache_cap  = 0;   /* allocated capacity */

static void ensure_p_cache(idx_t k)
{
    if ((size_t)k < p_cache_size) return;

    size_t new_cap = p_cache_cap == 0 ? 4 : p_cache_cap;
    while (new_cap <= (size_t)k) new_cap <<= 1;

    seq_t *old = p_cache;
    p_cache = (seq_t*)realloc(p_cache, new_cap * sizeof(seq_t));
    if (old == NULL && p_cache == NULL) return; /* allocation failed */

    if (p_cache_size == 0) {
        p_cache[0] = 1;
        p_cache[1] = 2;
        p_cache_size = 2;
    }

    size_t start = p_cache_size;
    if (start < 2) start = 2;
    for (size_t i = start; i <= (size_t)k; ++i) {
        p_cache[i] = 2 * p_cache[i - 1] + p_cache[i - 2];
    }
    p_cache_size = (size_t)k + 1;
    p_cache_cap  = new_cap;
}

static seq_t *a_cache   = NULL;   /* sequence a(n) */
static size_t a_cache_size = 0;   /* number of terms stored */
static size_t a_cache_cap  = 0;   /* allocated capacity */

static void ensure_a_cache(idx_t n)
{
    if (a_cache_size > n) return;

    ensure_p_cache(2 * n + 1);

    size_t new_cap = a_cache_cap == 0 ? 4 : a_cache_cap;
    while (new_cap <= (size_t)n + 1) new_cap <<= 1;

    seq_t *old = a_cache;
    a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (old == NULL && a_cache == NULL) return; /* allocation failed */

    size_t start = a_cache_size;
    if (start < 0) start = 0;
    for (idx_t i = start; i <= n; ++i) {
        seq_t term1 = ((seq_t)(14 * (idx_t)i + 13)) * p_cache[2 * (size_t)i + 1];
        seq_t term2 = ((seq_t)(6 * ((idx_t)i + 1))) * p_cache[2 * (size_t)i];
        a_cache[i] = (term1 + term2) / 32;
    }
    a_cache_size = (size_t)n + 1;
    a_cache_cap  = new_cap;
}

seq_t a(idx_t n)
{
    ensure_a_cache(n);
    return a_cache[n];
}
