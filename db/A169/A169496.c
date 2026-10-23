/*
 * A169496  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence a(0) = 1, a(n) = Σ_{k=1}^{33}
 *            a(n-k) – a(n-34) + b(n) (n ≥ 1) where b(0) = 1, b(1..33) = 2,
 *            b(34) = 1, b(n>34) = 0. This recurrence follows from the
 *            generating function (1+2t+…+2t³³+t⁴⁴)/(1−t−…−t³³+ t⁴⁴)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* cached values of a(n)          */
static size_t cache_cap = 0;          /* capacity of cache              */
static idx_t  computed_max = 0;       /* largest n computed so far      */

/* Ensure the cache can hold at least new_cap elements. */
static void
ensure_cap(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);           /* memory allocation failed */
    cache       = tmp;
    cache_cap   = new_cap;
}

/* Initialise cache for the first call. */
static void
init_cache(void)
{
    if (!cache) {
        cache       = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            exit(1);
        cache[0]    = 1;      /* a(0) */
        cache_cap   = 1;
        computed_max = 0;
    }
}

/* Return a(n). */
seq_t
a(idx_t n)
{
    init_cache();                  /* make sure a(0) is ready */

    if (n <= computed_max)        /* already known */
        return cache[(size_t)n];

    /* enlarge cache if necessary */
    size_t need = (size_t)(n + 1);
    ensure_cap(need);

    /* compute missing terms */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t val = 0;

        /* sum of previous 33 terms */
        for (idx_t k = 1; k <= 33; ++k)
            if (i >= k)
                val += cache[(size_t)(i - k)];

        /* subtract a(i-34) if applicable */
        if (i >= 34)
            val -= cache[(size_t)(i - 34)];

        /* add the b(i) contribution */
        if (i == 0)          val += (seq_t)1;
        else if (i <= 33)    val += (seq_t)2;
        else if (i == 34)    val += (seq_t)1;
        /* for i > 34 nothing is added */

        cache[(size_t)i] = val;
    }

    computed_max = n;
    return cache[(size_t)n];
}
