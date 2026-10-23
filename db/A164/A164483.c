/*
 * A164483  Number of binary strings of length n with no substrings equal to
 *          0001 0111 or 1010.
 * https://oeis.org/A164483
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 3*a(n-4) + a(n-6) Initial
 *            values: a(4)=13, a(5)=20, a(6)=30, a(7)=44, a(8)=64, a(9)=92,
 *            a(10)=131, a(11)=186, a(12)=263, a(13)=370 (values below the
 *            offset return 0)
 * Range:     u8 n <= 11, u16 n <= 28, u32 n <= 61, u64 n <= 128,
 *            u128 n <= 261, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* stored values */
static idx_t cache_cap = 0;          /* allocated indices 0 .. cache_cap-1 */
static idx_t computed_upto = 3;      /* highest index whose value is ready */

/* Grow the cache so that it can hold indices 0 .. n */
static void ensure_cache_capacity(idx_t n)
{
    if (n < cache_cap) return;

    idx_t old_cap = cache_cap;
    idx_t new_cap = (cache_cap ? cache_cap * 2 : 64);
    if (new_cap <= n) new_cap = n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    /* Zero the newly allocated part */
    for (idx_t i = old_cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* Put the known initial terms (4 ≤ n ≤ 13) into the cache */
static void initialize_base(idx_t up_to)
{
    idx_t max_base = up_to < 13 ? up_to : 13;
    /* 13 values from a(4) to a(13) */
    static const seq_t init_vals[] = { 13, 20, 30, 44, 64, 92, 131, 186, 263, 370 };
    for (idx_t i = 4; i <= max_base; ++i) {
        cache[i] = init_vals[i - 4];
    }
}

/* External function implementing the sequence */
seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;

    ensure_cache_capacity(n);

    /* Make sure base values are in place */
    if (computed_upto < 13 && n >= 4) {
        initialize_base( (n < 13) ? n : 13 );
        if (computed_upto < 13) computed_upto = (n < 13) ? n : 13;
    }

    if (n <= computed_upto) return cache[n];

    /* Compute recurrence for indices > 13 */
    idx_t start = computed_upto + 1;
    if (start <= 13) start = 14;  /* indices 4..13 already initialized */

    for (idx_t k = start; k <= n; ++k) {
        /* a(k) = 2*a(k-1) - a(k-2) + 2*a(k-3) - 3*a(k-4) + a(k-6) */
        seq_t t1 = (seq_t)2 * cache[k - 1] + (seq_t)2 * cache[k - 3] + cache[k - 6];
        seq_t t2 = cache[k - 2] + (seq_t)3 * cache[k - 4];
        cache[k] = t1 - t2;
    }
    computed_upto = n;
    return cache[n];
}
