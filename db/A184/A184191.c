/*
 * A184191  Half the number of (n+1)X4 binary arrays with no 2X2 subblock
 *          containing exactly one 1.
 * https://oeis.org/A184191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 14*a(n-1) - 45*a(n-2) - 18*a(n-3) + 249*a(n-4)
 *              - 239*a(n-5) - 128*a(n-6) + 184*a(n-7) + 10*a(n-8)
 *              - 20*a(n-9)  - 2*a(n-10)
 * Initial values (n = 1 … 10):
 * 60, 561, 5074, 46514, 425802, 3901193, 35741495,
 * 327471410, 3000373034, 27490276713
 *
 * The function implements the recurrence with memoisation.
 */

#include <stdlib.h>

/* static storage for memoisation */
static seq_t *cache = NULL;          /* array of computed terms */
static idx_t  cache_cap = 0;         /* number of allocated slots (>=0) */
static idx_t  cache_len = 0;         /* largest index for which cache[i] is valid */

/* immutable list of the first ten terms (1‑indexed) */
static const seq_t init[11] = {
    0,              /* unused 0th element */
    60,            /* a(1) */
    561,           /* a(2) */
    5074,          /* a(3) */
    46514,         /* a(4) */
    425802,        /* a(5) */
    3901193,       /* a(6) */
    35741495,      /* a(7) */
    327471410,     /* a(8) */
    3000373034,    /* a(9) */
    27490276713    /* a(10) */
};

/* initialise the cache to hold the known first ten terms */
static void
ensure_initialised(void)
{
    if (cache_len >= 10) return;   /* already done */

    /* allocate at least ten slots */
    if (cache_cap < 11) {
        idx_t new_cap = 11;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {   /* allocation failed – abort with undef behaviour */
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = 1; i <= 10; ++i)
        cache[i] = init[i];

    cache_len = 10;
}

/* extend the cache up to at least 'n' */
static void
grow_cache(idx_t n)
{
    if (n <= cache_len) return;

    if (cache_cap < n + 1) {
        idx_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* memory error – aborting */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute remaining terms via recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 10) {             /* safety guard: should not occur */
            cache[i] = init[i];
            continue;
        }

        seq_t pos = (seq_t)14 * cache[i - 1] +
                    (seq_t)249 * cache[i - 4] +
                    (seq_t)184 * cache[i - 7] +
                    (seq_t)10  * cache[i - 8];

        seq_t neg = (seq_t)45  * cache[i - 2] +
                    (seq_t)18  * cache[i - 3] +
                    (seq_t)239 * cache[i - 5] +
                    (seq_t)128 * cache[i - 6] +
                    (seq_t)20  * cache[i - 9] +
                    (seq_t)2   * cache[i -10];

        cache[i] = pos - neg;
    }

    cache_len = n;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;    /* undefined, but safe guard */

    ensure_initialised();
    grow_cache(n);
    return cache[n];
}
