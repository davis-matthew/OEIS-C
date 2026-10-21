/*
 * A069987  Squarefree numbers of form k^2 + 1.
 * https://oeis.org/A069987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A049533(n)^2 + 1
 * Range:     u8 n <= 14, u16 n <= 228, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the (n‑th) square‑free integer of the form k²+1. */

#include <stdlib.h>
#include <stddef.h>

/* ---------- primes up to a fixed bound --------------------------------- */
static unsigned long long *primes = NULL;
static size_t primes_len    = 0;
static size_t primes_cap    = 0;

static void init_primes(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    /* upper bound sufficient for k up to ~200 000 (more than needed for n≈10⁴) */
    const unsigned long long LIMIT = 200000;

    unsigned char *composite = (unsigned char*)calloc(LIMIT + 1, 1);
    if (!composite) return; /* allocation failed – we’ll just skip sieve */

    for (unsigned long long i = 2; i * i <= LIMIT; ++i)
        if (!composite[i])
            for (unsigned long long j = i * i; j <= LIMIT; j += i)
                composite[j] = 1;

    /* count primes and allocate array */
    size_t count = 0;
    for (unsigned long long i = 2; i <= LIMIT; ++i)
        if (!composite[i]) ++count;

    primes_cap = count;
    primes = (unsigned long long*)malloc(primes_cap * sizeof(unsigned long long));
    if (!primes) { free(composite); return; }

    primes_len = 0;
    for (unsigned long long i = 2; i <= LIMIT; ++i)
        if (!composite[i]) primes[primes_len++] = i;

    free(composite);
}

/* ---------- square‑free test for 64‑bit integers ---------------------- */
static int is_squarefree_ull(unsigned long long m)
{
    init_primes();  /* guarantees primes[] is ready */
    for (size_t i = 0; i < primes_len; ++i)
    {
        unsigned long long p = primes[i];
        if (p * p > m) break;
        if (m % p == 0)
        {
            m /= p;
            if (m % p == 0) return 0;          /* p² divides m */
        }
    }
    return 1;
}

/* ---------- cached sequence -------------------------------------------- */
static seq_t *cache   = NULL;   /* 0 = uninitialised */
static size_t cache_len = 0;
static size_t cache_cap = 0;
static unsigned long long last_k = 0;  /* last k tested */

/* enlarge cache to at least n elements */
static void ensure_cache(size_t n)
{
    if (cache_len >= n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n) new_cap *= 2;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) return; /* allocation failed – leave cache unchanged */
    cache     = new_cache;
    cache_cap = new_cap;
}

/* ---------- public function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behaviour below offset irrelevant */

    ensure_cache((size_t)n);

    while (cache_len < (size_t)n)
    {
        ++last_k;
        unsigned long long m = last_k * last_k + 1ULL;      /* integer m */
        if (is_squarefree_ull(m))
        {
            /* store m in cache – cast to seq_t is safe for the sizes we hit */
            cache[cache_len++] = (seq_t)m;
        }
    }
    return cache[n - 1];
}
