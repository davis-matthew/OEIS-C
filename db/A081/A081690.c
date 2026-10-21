/*
 * A081690  From P-positions in a certain game.
 * https://oeis.org/A081690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = mex{a(i), a(i)+2^i-1 : 0 <= i < n}, b(0)=0,
 *            b(n)=a(n)+2^n-1
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation pre‑computes the sequence on demand and memoises
   values.  A small bool array marks all values that have occurred
   up to the current index so that the mex can be found in O(1) amortised
   time.  Only the first few values of b(n) (those with 2^n <= 2^15)
   are stored in the “seen” array; larger powers of two are too big to
   influence the mex.  The code works for all provided seq_t / idx_t
   types. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Size of the boolean array that records which integers have already
   appeared in {a(i)}∪{b(i)}.  Must be larger than the largest
   mex that can ever occur.  For n up to 10 000 the mex never
   exceeds about 2 n < 20 000; a capacity of 200 000 is more
   than sufficient for any expected input size. */
#define SEEN_CAP 200000

static unsigned char seen[SEEN_CAP];          /* 0 = not seen, 1 = seen */
static size_t seen_cap = SEEN_CAP;             /* current capacity, kept for consistency */

static seq_t *cache = NULL;      /* memoised a(n) values */
static size_t cache_len = 0;     /* number of values currently stored */
static size_t cache_cap = 0;     /* allocated capacity of cache */

static seq_t next_mex = 0;       /* smallest non‑negative integer not yet seen */

/* initialise the sequence caches on first call */
static void seq_a_init(void)
{
    /* Allocate initial cache space */
    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) {
        /* allocation failure – in a contest environment we abort */
        abort();
    }

    /* a(0)=0, b(0)=0  */
    cache[0] = (seq_t)0;
    cache_len = 1;

    /* mark 0 as seen */
    seen[0] = 1;
    /* next_mex scans for first unseen integer */
    while (next_mex < seen_cap && seen[(size_t)next_mex]) {
        next_mex++;
    }
}

/* add a new value a_k to the cache and mark all relevant entries
   in the seen array.  k is the index being added. */
static void seq_a_add(idx_t k)
{
    /* find mex for this k */
    while (next_mex < seen_cap && seen[(size_t)next_mex]) {
        next_mex++;
    }
    seq_t ak = next_mex;

    /* store into cache */
    cache[cache_len++] = ak;

    /* mark a_k as seen if it fits in the array */
    if ((size_t)ak < seen_cap) {
        seen[(size_t)ak] = 1;
    }

    /* compute b_k = a_k + 2^k - 1  and mark if it fits */
    if (k <= 15) {   /* 2^15 = 32768, beyond our seen array we ignore */
        seq_t pow2 = ((seq_t)1) << (unsigned)k;   /* 1 << k is safe for k <= 15 */
        seq_t bk = ak + pow2 - (seq_t)1;
        if ((size_t)bk < seen_cap) {
            seen[(size_t)bk] = 1;
        }
    }
}

/* ensure there is enough capacity in the cache for at least n+1 entries */
static void seq_a_ensure_capacity(idx_t n)
{
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort();  /* allocation failure */
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* public function: returns a(n)  */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (cache == NULL) {
        seq_a_init();
    }

    /* extend cache if necessary */
    if ((size_t)n >= cache_cap) {
        seq_a_ensure_capacity(n);
    }

    /* compute missing values up to n */
    while (cache_len <= (size_t)n) {
        seq_a_add((idx_t)(cache_len));
    }

    return cache[(size_t)n];
}
