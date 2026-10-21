/*
 * A225975  Square root of A226008(n).
 * https://oeis.org/A225975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A225975: a(0)=0, a(1)=2, a(2)=2, a(3)=6, a(4)=1, a(5)=10, a(6)=6, a(7)=14, a(8)=4, a(9)=18,
   a(10)=10, a(11)=22, a(12)=3, a(13)=26, a(14)=14, a(15)=30.
   For n≥16 the recurrence
          a(n) = 2·a(n‑8) – a(n‑16)
   holds.  The function below memoises the values in a static cache. */
#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

static const seq_t BASE[16] = {
    (seq_t)0,  (seq_t)2,  (seq_t)2,  (seq_t)6,
    (seq_t)1,  (seq_t)10, (seq_t)6,  (seq_t)14,
    (seq_t)4,  (seq_t)18, (seq_t)10, (seq_t)22,
    (seq_t)3,  (seq_t)26, (seq_t)14, (seq_t)30
};

static seq_t *CACHE = NULL;
static size_t CACHE_SIZE = 0;   /* number of valid entries */
static size_t CACHE_CAP  = 0;   /* allocated capacity */

/* Ensure that CACHE contains values 0 … n */
static void ensure_cache(idx_t n)
{
    size_t target = (size_t)n;            /* target index */
    if (target + 1 <= CACHE_SIZE) return; /* already available */

    if (CACHE == NULL) {
        /* Initial allocation */
        CACHE_CAP = (target + 1 > 16 ? target + 1 : 16);
        CACHE     = alloc_seq(CACHE_CAP);
        for (size_t i = 0; i < 16 && i < CACHE_CAP; ++i)
            CACHE[i] = BASE[i];
        CACHE_SIZE = (CACHE_CAP < 16 ? CACHE_CAP : 16);
    }
    else if (target + 1 > CACHE_CAP) {
        /* Grow capacity to at least target+1, doubling each step */
        size_t new_cap = CACHE_CAP;
        while (new_cap <= target) new_cap <<= 1;
        seq_t *new_cache = alloc_seq(new_cap);
        for (size_t i = 0; i < CACHE_SIZE; ++i)
            new_cache[i] = CACHE[i];
        free_seq(CACHE);
        CACHE = new_cache;
        CACHE_CAP = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = CACHE_SIZE; i <= target; ++i) {
        if (i < 16) {
            CACHE[i] = BASE[i];
        } else {
            /* a(i) = 2*a(i-8) - a(i-16) */
            CACHE[i] = (CACHE[i - 8] << 1) - CACHE[i - 16];
        }
    }
    CACHE_SIZE = target + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return CACHE[(size_t)n];
}
