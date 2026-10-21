/*
 * A224034  Number of nX4 0..1 arrays with antidiagonals unimodal and rows and
 *          diagonals nondecreasing.
 * https://oeis.org/A224034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) - a(n-3) + 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 4*a(n-7) Initial terms: a(1)=5, a(2)=19, a(3)=58,
 *            a(4)=168, a(5)=488, a(6)=1422, a(7)=4160
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache: index 0 is unused to keep access 1‑based */
static seq_t *cache      = NULL;
static idx_t  cached_to = 0;    /* highest index already stored (≥0) */
static size_t cache_cap  = 0;    /* current allocated size (≥0) */

static const seq_t base_terms[8] = {
    (seq_t)0,  /* unused */
    (seq_t)5,  (seq_t)19, (seq_t)58, (seq_t)168,
    (seq_t)488, (seq_t)1422, (seq_t)4160
};

/* Ensure that cache has space for indices 1 … n.  The array is zero‑upper‑case. */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    /* Allocate an array of size n+1 (index 0 .. n) */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp)
        abort();                     /* fatal memory error */
    cache = tmp;
    cache_cap = (size_t)(n + 1);
}

/* Initialize the first seven terms if they are not yet stored. */
static void init_base(void)
{
    if (cached_to >= 7)
        return;

    /* Ensure we have room for indices 1…7 */
    ensure_capacity(7);
    for (idx_t i = 1; i <= 7; ++i)
        cache[i] = base_terms[i];
    cached_to = 7;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0 – return 0 */

    ensure_capacity(n);
    init_base();

    if (n <= cached_to)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = cached_to + 1; i <= n; ++i) {
        seq_t pos = 5 * cache[i - 1];
        pos += 2 * cache[i - 4];
        pos += 2 * cache[i - 5];
        pos += 4 * cache[i - 7];

        seq_t sub = 6 * cache[i - 2];
        sub += 1 * cache[i - 3];
        sub += 2 * cache[i - 6];

        cache[i] = pos - sub;   /* unsigned arithmetic, safe because pos ≥ sub */
    }
    cached_to = n;
    return cache[n];
}
