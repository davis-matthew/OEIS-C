/*
 * A084936  Nonsquarefree numbers divided by their squarefree kernels.
 * https://oeis.org/A084936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003557(A013929(n))
 * Range:     u8 n <= 197, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th non‑squarefree integer divided by its squarefree kernel */

#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Static data                                                        */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;       /* cache of a-values (a[0] = a(1))          */
static idx_t cache_size = 0;      /* allocated cache size                     */
static idx_t computed_n = 0;      /* largest n for which a(n) is cached       */
static idx_t last_m = 1;          /* last integer examined for non‑squarefree */

static idx_t *spf = NULL;         /* smallest‑prime‑factor array              */
static idx_t spf_limit = 0;       /* maximum index such that spf[m] is valid   */

/* --------------------------------------------------------------------- */
/*  Smallest‑prime‑factor sieve                                        */
/* --------------------------------------------------------------------- */
static void init_spf(idx_t limit)
{
    /* allocate/resize the array */
    spf = (idx_t *)realloc(spf, (limit + 1) * sizeof(idx_t));
    if (!spf) exit(1);

    /* initial values */
    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;

    /* sieve */
    for (idx_t i = 2; i <= limit / i; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
    spf_limit = limit;
}

/* ensure that spf is valid up to at least 'limit' */
static void ensure_spf_limit(idx_t limit)
{
    if (limit <= spf_limit) return;
    idx_t new_limit = limit * 2;
    if (new_limit < limit || new_limit == 0) new_limit = limit;  /* overflow guard */
    init_spf(new_limit);
}

/* --------------------------------------------------------------------- */
/*  Cache handling                                                    */
/* --------------------------------------------------------------------- */
static void ensure_cache(idx_t need)
{
    if (need <= cache_size) return;
    idx_t new_size = cache_size ? cache_size * 2 : 64;
    while (new_size < need) new_size <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(1);
    cache = new_cache;
    cache_size = new_size;
}

/* --------------------------------------------------------------------- */
/*  Compute m / rad(m) for a (possibly squarefree) integer m           */
/* --------------------------------------------------------------------- */
static seq_t quotient_from_n(idx_t m)
{
    seq_t q = 1;
    idx_t tmp = m;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        if (e > 1) {
            for (int k = 1; k < e; ++k) q *= (seq_t)p;
        }
    }
    return q;   /* for squarefree numbers q == 1 */
}

/* --------------------------------------------------------------------- */
/*  Public sequence function                                           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                          /* undefined, just avoid UB */
    if (n <= computed_n) return cache[n - 1];      /* cached value */

    ensure_cache(n);                               /* make sure cache is large enough */

    idx_t m = last_m;
    for (idx_t i = computed_n + 1; i <= n; ++i) {
        seq_t q = 0;
        while (1) {
            ++m;
            ensure_spf_limit(m);          /* make sure spf is ready */
            q = quotient_from_n(m);       /* 1 ↔ squarefree, >1 ↔ non‑squarefree */
            if (q > 1) {
                cache[i - 1] = q;         /* store a(i) */
                break;
            }
        }
    }

    computed_n = n;
    last_m = m;
    return cache[n - 1];
}
