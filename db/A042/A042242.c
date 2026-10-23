/*
 * A042242  Numerators of continued fraction convergents to sqrt(647).
 * https://oeis.org/A042242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Sequence: numerators of convergents to sqrt(647)
   Recurrence: a(0)=25; a(1)=51; for n≥2,
   a(n)=c(n)*a(n-1)+a(n-2)
   where c(n) is the nth partial quotient of the continued fraction of √647
   (a0=25, a1=2, a2=3 …).  The function builds a cache and
   generates new partial quotients on demand. */

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* state of the continued‑fraction algorithm */
static int m   = 0;
static int d   = 1;
static int ak  = 0;      /* current a_k ready for the next term   */
static bool initialized = false;

/* enlarge the cache if needed */
static void ensure_capacity(idx_t needed)
{
    (void)needed; /* unused */
    if (cache_len >= cache_cap)
    {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* initialise the cache with a(0) and state for a(1) */
static void init_cache(void)
{
    if (initialized) return;
    initialized = true;

    cache_cap = 32;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));

    cache[0] = (seq_t)25;           /* a(0) */
    cache_len = 1;

    /* compute a1 from the standard recurrence for sqrt(D) */
    int m_next = d * 25 - m;                /* 1   */
    int d_next = (647 - m_next * m_next) / d; /* 22  */
    int a_next = (25 + m_next) / d_next;      /* 2   */

    m  = m_next;
    d  = d_next;
    ak = a_next;
}

/* grow the cache up to index n */
static void grow_cache(idx_t n)
{
    while (cache_len <= n)
    {
        seq_t prev1 = cache[cache_len - 1];
        seq_t prev2 = (cache_len >= 2) ? cache[cache_len - 2] : (seq_t)1;

        seq_t next_val = (seq_t)ak * prev1 + prev2;

        ensure_capacity(cache_len + 1);
        cache[cache_len] = next_val;
        cache_len++;

        /* advance to next partial quotient */
        int m_next = d * ak - m;
        int d_next = (647 - m_next * m_next) / d;
        int a_next = (25 + m_next) / d_next;

        m  = m_next;
        d  = d_next;
        ak = a_next;
    }
}

seq_t a(idx_t n)
{
    init_cache();
    if (n >= cache_len) grow_cache(n);
    return cache[n];
}
