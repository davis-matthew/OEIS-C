/*
 * A137725  Number of sequences of length n with elements {-2,-1,+1,+2}, such
 *          that the sum of elements of the whole sequence but of no proper
 *          subsequence equals 0 modulo n. For n>=4, the number of Hamiltonian
 *          (directed) circuits on the circulant graph C_n(1,2).
 * https://oeis.org/A137725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 27, u32 n <= 56, u64 n <= 114,
 *            u128 n <= 230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A137725: Number of sequences of length n with elements {-2,-1,+1,+2}
    such that the sum of elements of the whole sequence but of no proper
    subsequence equals 0 modulo n.
    For n>8 a(n) = 2*a(n−1) − a(n−3) − a(n−5) + a(n−6)
    Base values (n=1..8):
    4, 4, 16, 18, 24, 32, 46, 58
*/
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms, 1‑based indexing */
static size_t cache_size = 0;        /* number of allocated slots */
static size_t computed_len = 0;      /* index of the last computed term */

static void ensure_cache(size_t n)
{
    if (n <= cache_size)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (new_cache == NULL)
        exit(1);                      /* allocation failed */
    /* initialise new slots to 0 (uncomputed) */
    for (size_t i = cache_size; i < n; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_size = n;
}

static void init_base_terms(void)
{
    if (computed_len >= 8)
        return;                       /* base terms already present */
    ensure_cache(8);
    cache[0] = 4;   /* a(1) */
    cache[1] = 4;   /* a(2) */
    cache[2] = 16;  /* a(3) */
    cache[3] = 18;  /* a(4) */
    cache[4] = 24;  /* a(5) */
    cache[5] = 32;  /* a(6) */
    cache[6] = 46;  /* a(7) */
    cache[7] = 58;  /* a(8) */
    computed_len = 8;
}

static void compute_up_to(idx_t n)
{
    if (n <= computed_len)
        return;
    ensure_cache((size_t)n);
    init_base_terms();                /* guarantees base terms are set */
    for (size_t i = computed_len + 1; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) - a(i-3) - a(i-5) + a(i-6)   (i>=9) */
        seq_t val = 2 * cache[i - 2];          /* 2*a(i-1) */
        val -= cache[i - 4];                   /* - a(i-3) */
        val -= cache[i - 6];                   /* - a(i-5) */
        val += cache[i - 7];                   /* + a(i-6) */
        cache[i - 1] = val;                    /* store a(i) */
    }
    computed_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n < 1 */
    compute_up_to(n);
    return cache[(size_t)n - 1];
}
