/*
 * A098905  Odd numbers whose number of distinct prime factors is positive and
 *          even.
 * https://oeis.org/A098905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A098905: odd numbers whose number of distinct prime factors is positive and even. */
/* The function scans odd integers in increasing order, using a smallest‑prime‑factor sieve
   to count distinct prime factors. Results are memoised so that calls in any order are
   served efficiently. */

#include <stdlib.h>
#include <stdbool.h>

static idx_t cached_len   = 0;    /* number of terms stored (1‑based) */
static idx_t cached_cap   = 0;    /* allocated capacity of cache */
static seq_t *cache        = NULL;/* array of sequence terms */

static bool    sieve_done   = false;
static idx_t * spf           = NULL;
static idx_t   sieve_limit  = 0;  /* current sieve size (exclusive) */
static idx_t   last_checked = 3;  /* next odd number to test */

/* initialise the smallest‑prime‑factor sieve once */
static void init_sieve(void)
{
    if (sieve_done) return;

    /* choose an initial sieve limit that comfortably contains >10 000 terms */
    sieve_limit = 200001;          /* exclusive upper bound */

    spf = (idx_t*)malloc(sieve_limit * sizeof(idx_t));
    if (!spf) return;              /* allocation failure – the function will return 0 */

    for (idx_t i = 0; i < sieve_limit; ++i) spf[i] = i;

    for (idx_t i = 2; i * i < sieve_limit; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j < sieve_limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    sieve_done = true;
}

/* push a new value into the cache, resizing if needed */
static void push_cache(seq_t val)
{
    if (cached_len == cached_cap) {
        size_t new_cap = cached_cap ? cached_cap * 2 : 64;
        seq_t *new_buf = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_buf) return;          /* allocation failure – abort */
        cache = new_buf;
        cached_cap = new_cap;
    }
    cache[cached_len++] = val;
}

/* enlarge the sieve to at least new_limit, recomputing spf for all indices */
static bool enlarge_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit) return true;  /* nothing to do */

    idx_t *new_spf = (idx_t*)realloc(spf, new_limit * sizeof(idx_t));
    if (!new_spf) return false;                 /* allocation failure */

    /* initialise new entries */
    for (idx_t i = sieve_limit; i < new_limit; ++i) new_spf[i] = i;

    /* recompute spf for the entire range */
    for (idx_t i = 2; i * i < new_limit; ++i) {
        if (new_spf[i] == i) {
            for (idx_t j = i * i; j < new_limit; j += i)
                if (new_spf[j] == j) new_spf[j] = i;
        }
    }

    spf = new_spf;
    sieve_limit = new_limit;
    return true;
}

/* compute a(n), expanding the cache as needed */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n = 0 */

    init_sieve();
    if (!spf) return (seq_t)0;           /* initialization failed */

    /* extend cache until we have at least n terms */
    while (cached_len < n) {
        if (last_checked >= sieve_limit) {
            /* grow the sieve (default double the size) */
            if (!enlarge_sieve(sieve_limit * 2)) return (seq_t)0;
        }

        /* count distinct prime factors of last_checked */
        idx_t tmp = last_checked;
        idx_t cnt = 0;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            ++cnt;
            while (tmp % p == 0) tmp /= p;
        }

        if (cnt > 0 && (cnt & 1) == 0)   /* positive and even */
            push_cache((seq_t)last_checked);

        last_checked += 2;               /* move to next odd */
    }

    return cache[n - 1];
}
