/*
 * A232616  Least positive integer m such that {2^k - k: k = 1,...,m} contains
 *          a complete system of residues modulo n.
 * https://oeis.org/A232616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 5658, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A232616: 
   a(n) = smallest m such that the set { 2^k – k : k = 1..m } contains
   a complete system of residues modulo n. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* cache of already computed a(n) values (1‑based index) */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;

/* ensure that cache can hold indices up to n */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n > cache_cap)
    {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* memory exhausted */
        /* initialise new entries to 0 (meaning “not yet computed”) */
        memset(tmp + cache_cap, 0,
               (new_cap - cache_cap) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* auxiliary byte array for residue bookkeeping */
static uint32_t *marker   = NULL;
static size_t    mark_cap = 0;
static uint32_t  current_token = 1;  /* 0 is reserved for “uninitialised” */

/* ensure marker array can hold indices up to n */
static void
ensure_marker(idx_t n)
{
    if ((size_t)n > mark_cap)
    {
        size_t new_cap = (size_t)n;
        uint32_t *tmp = (uint32_t *)realloc(marker, new_cap * sizeof(uint32_t));
        if (!tmp) abort();                                /* memory exhausted */
        if (new_cap > mark_cap)
            memset(tmp + mark_cap, 0,
                   (new_cap - mark_cap) * sizeof(uint32_t));
        marker = tmp;
        mark_cap = new_cap;
    }
}

/* reset the marker array when the token counter overflows (very unlikely) */
static void
reset_marker(void)
{
    memset(marker, 0, mark_cap * sizeof(uint32_t));
    current_token = 1;
}

/* ------------------------------------------------------------ */
/* main routine computing A232616                              */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n = 0 */

    ensure_cache(n);
    if (cache[n-1] != 0) return cache[n-1]; /* already computed */

    /* prepare marker array */
    ensure_marker(n);

    /* bump token counter for this run */
    ++current_token;
    if (current_token == 0) reset_marker();

    size_t seen = 0;
    seq_t n_seq = (seq_t)n;               /* make n a seq_t for modulo ops */
    seq_t cur   = 1 % n_seq;              /* 2^0 mod n */
    idx_t k = 1;
    for (;; ++k)
    {
        cur = (cur * (seq_t)2) % n_seq;   /* cur = 2^k mod n */

        seq_t kmod = (seq_t)(k % n);
        seq_t val  = cur >= kmod ? cur - kmod : cur + n_seq - kmod;
        size_t idx = (size_t)val;         /* 0 ≤ val < n */

        if (marker[idx] != current_token)
        {
            marker[idx] = current_token;
            ++seen;
            if (seen == n)
            {
                cache[n-1] = (seq_t)k;
                return (seq_t)k;
            }
        }
    }
}
