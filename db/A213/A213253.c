/*
 * A213253  a(n) = smallest k such that highest prime factor of
 *          m(m+1)...(m+k-1) is > n if m > n.
 * https://oeis.org/A213253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 268, u16 n <= 268, u32 n <= 268, u64 n <= 268,
 *            u128 n <= 268, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A213253: a(n) = 1 + maximum length of consecutive integers m > n
 *   such that the largest prime factor of each m lies ≤ n.
 *
 * The algorithm pre‑computes the largest prime factor for all integers
 * up to a fixed global limit (1 000 000).  For each n we scan forward
 * from m = n+1 until we reach an integer with a larger prime factor; the
 * longest run found gives a(n) = length + 1.  Results are cached so
 * repeated calls are O(1).  The code works for both C and C++ compilers.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static uint64_t *lpf_tbl = NULL;          /* largest prime factor table   */
static const idx_t LPF_LIMIT = 1000000;   /* precomputation limit        */

/* initialise lpf_tbl */
static void init_lpf_tbl(void)
{
    if (lpf_tbl != NULL) return;

    lpf_tbl = (uint64_t*)calloc((size_t)(LPF_LIMIT + 1), sizeof(uint64_t));
    if (!lpf_tbl) return;              /* allocation failed */

    for (idx_t p = 2; p <= LPF_LIMIT; ++p)
    {
        if (lpf_tbl[p] == 0)            /* p is prime              */
        {
            for (idx_t m = p; m <= LPF_LIMIT; m += p)
                lpf_tbl[m] = p;         /* later (larger) primes override */
        }
    }
}

/* cached sequence values */
static seq_t *seq_cache = NULL;   /* seq_cache[n] holds a(n)   */
static size_t cache_size = 0;

static void ensure_cache_size(idx_t n)
{
    if (cache_size > (size_t)n) return;

    size_t new_size = (size_t)n + 16;  /* small growth factor      */
    seq_t *new_cache = (seq_t*)realloc(seq_cache, new_size * sizeof(seq_t));
    if (!new_cache) return;            /* allocation failed: keep old */

    memset(new_cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
    seq_cache = new_cache;
    cache_size = new_size;
}

/* main sequence function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of range, harmless */

    /* initialise data structures lazily */
    init_lpf_tbl();
    if (seq_cache == NULL)
    {
        seq_cache = (seq_t*)calloc(1, sizeof(seq_t));
        cache_size = 1;
    }
    ensure_cache_size(n);

    /* cached value? */
    if (seq_cache[n] != (seq_t)0) return seq_cache[n];

    /* compute a(n) by scanning for longest block with lpf ≤ n */
    seq_t best = 0;    /* longest run found */
    seq_t cur  = 0;    /* current run length */

    idx_t start = n + 1;
    if (start <= LPF_LIMIT)
    {
        for (idx_t m = start; m <= LPF_LIMIT; ++m)
        {
            uint64_t lpf = lpf_tbl[m];
            if (lpf <= n)
            {
                ++cur;
                if (cur > best) best = cur;
                if (best == n - 1) break;   /* can't be longer */
            }
            else
            {
                cur = 0;
            }
        }
    }

    seq_t result = best + 1;          /* a(n) = max run + 1          */
    seq_cache[n] = result;
    return result;
}
