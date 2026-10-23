/*
 * A042664  Numerators of continued fraction convergents to sqrt(862).
 * https://oeis.org/A042664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=29, a(1)=59; a(n)=q[(n-1)%40]*a(n-1)+a(n-2) with q the
 *            continued‑fraction partial quotients of sqrt(862)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 16, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Memoisation cache for the sequence values. */
static seq_t *cache = NULL;           /* cache[0]..cache[cache_max] are valid */
static size_t cache_cap = 0;          /* allocated array size */
static idx_t cache_max = 0;           /* highest computed index */

/* Table of the 40 continued‑fraction partial quotients after a0 = 29. */
static unsigned int *q = NULL;
static size_t q_len = 0;

/* Initialise the q table (runs only once). */
static void init_q(void)
{
    if (q != NULL)
        return;   /* already initialised */

    const unsigned int D  = 862;
    const unsigned int a0 = 29;

    unsigned int m = 0;
    unsigned int d = 1;
    unsigned int a = a0;

    /* Allocate a bit more than enough; the period is 40. */
    q = (unsigned int*)malloc(128 * sizeof(unsigned int));
    if (!q)
        exit(1);

    size_t pos = 0;
    while (1) {
        /* Standard continued fraction step. */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        q[pos++] = a;
        if (a == 2 * a0)   /* period terminates when a == 2*a0 */
            break;
    }
    q_len = pos;  /* should be 40 */
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    init_q();  /* Ensure the q table is ready. */

    /* n == 0 is handled directly; this also guarantees the cache has a(0) ready. */
    if (n == 0)
        return (seq_t)29;

    /* First initialisation of cache: store a(0) and a(1). */
    if (cache == NULL) {
        cache_cap = 2;                            /* need indices 0 and 1 */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(1);
        cache[0] = (seq_t)29;                     /* a(0) */
        cache[1] = (seq_t)59;                     /* a(1) */
        cache_max = 1;
    }

    /* Grow the cache array if required. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the value is already cached, return it immediately. */
    if (n <= cache_max)
        return cache[(size_t)n];

    /* Set the two previous terms for the recurrence. */
    seq_t pm2 = cache[cache_max - 1];  /* a(n-2) */
    seq_t pm1 = cache[cache_max];      /* a(n-1) */

    /* Compute successive terms up to n. */
    for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
        unsigned int qi = q[(i - 1) % q_len];

#if defined(__cplusplus)
        /* In C++ mode (including arbitrary‑precision types) use normal arithmetic. */
        seq_t val = (seq_t)qi * pm1 + pm2;
#else
        /* In plain C mode, use unsigned __int128 to avoid overflow. */
        unsigned __int128 prod = (unsigned __int128)qi * (unsigned __int128)pm1;
        prod += (unsigned __int128)pm2;
        seq_t val = (seq_t)prod;
#endif
        cache[i] = val;
        pm2 = pm1;
        pm1 = val;
        cache_max = (idx_t)i;
    }

    return cache[(size_t)n];
}
