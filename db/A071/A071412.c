/*
 * A071412  A002487 mod 3.
 * https://oeis.org/A071412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Stern's diatomic sequence modulo 3 (recurrence: a(0)=0,
 *            a(1)=1; for n≥2: if n even → a(n)=a(n/2); if n odd →
 *            a(n)=(a((n-1)/2)+a((n+1)/2)) mod 3)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of already computed a(i) mod 3 */
static size_t cap = 0;        /* current allocated capacity (bytes of seq_t) */
static idx_t computed_up_to = 0;  /* largest index that has been filled */

/* enlarge the cache so that indices up to n are safe */
static void ensure_cap(idx_t n)
{
    if (n + 1 > cap) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }
}

/* initialise the cache with the first two values of the sequence */
static void init_cache(void)
{
    if (cache == NULL) {
        size_t init_cap = 16;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        cap = init_cap;
        cache[0] = 0;
        cache[1] = 1;
        computed_up_to = 1;   /* we now have a(0) and a(1) */
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (computed_up_to == 0)   /* first call -> allocate & initialise */
        init_cache();

    if (n > computed_up_to) {  /* need to extend the table */
        ensure_cap(n);
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            if ((i & 1) == 0) {               /* even */
                cache[i] = cache[i >> 1];     /* a(i) = a(i/2) */
            } else {                          /* odd  */
                seq_t left  = cache[(i - 1) >> 1];
                seq_t right = cache[(i + 1) >> 1];
                cache[i] = (left + right) % (seq_t)3;
            }
        }
        computed_up_to = n;
    }
    return cache[n];
}
