/*
 * A228279  Number of n X 4 binary arrays with top left value 1 and no two
 *          ones adjacent horizontally, vertically or nw-se diagonally.
 * https://oeis.org/A228279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A228279: Number of n×4 binary arrays with top‑left value 1
   and no two 1‑digits adjacent horizontally, vertically or northwest‑south‑east diagonally.

   Recurrence (OEIS):
   a(n) = a(n‑1) + 10·a(n‑2) + 15·a(n‑3) + 4·a(n‑4)
          – 6·a(n‑5) – a(n‑6) + 3·a(n‑7) – a(n‑8)

   Initial values: a(1)=3, a(2)=6, a(3)=35, a(4)=133,
                   a(5)=587, a(6)=2448, a(7)=10414, a(8)=44024.
*/

#include <stdlib.h>
#include <stddef.h>

/* --- dynamic cache --------------------------------------------------- */

static seq_t *cache      = NULL;  /* 1‑based array of known values */
static size_t cache_size = 0;     /* number of allocated slots (including a[0]) */
static int    initialized = 0;    /* whether the first 8 terms are stored */

/* ensure that cache can address indices up to idx (inclusive) */
static void
ensure_capacity(size_t idx)
{
    if (idx < cache_size)
        return;

    /* grow to idx+1 slots, index 0 will stay unused */
    size_t new_size = idx + 1;
    seq_t *new_buf = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_buf) {
        /* out of memory – terminate immediately */
        exit(EXIT_FAILURE);
    }

    /* zero the newly allocated part */
    for (size_t i = cache_size; i < new_size; ++i)
        new_buf[i] = 0;

    cache      = new_buf;
    cache_size = new_size;
}

/* store the first 8 known terms */
static void
init_cache(void)
{
    if (initialized)
        return;
    ensure_capacity(8);          /* indices 0..8 */
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)6;
    cache[3] = (seq_t)35;
    cache[4] = (seq_t)133;
    cache[5] = (seq_t)587;
    cache[6] = (seq_t)2448;
    cache[7] = (seq_t)10414;
    cache[8] = (seq_t)44024;
    initialized = 1;
}

/* --- public API ------------------------------------------------------ */

seq_t
a(idx_t n)
{
    if (n < 1)          /* behaviour for n<1 is irrelevant */
        return 0;

    init_cache();               /* guarantee base values are set */
    ensure_capacity((size_t)n); /* grow cache if required */

    /* if already computed, return immediately */
    if (cache[n] != 0)
        return cache[n];

    /* compute missing values up to n */
    for (size_t i = 9; i <= (size_t)n; ++i) {
        seq_t term =   cache[i-1]
                    + ((seq_t)10) * cache[i-2]
                    + ((seq_t)15) * cache[i-3]
                    + ((seq_t)4 ) * cache[i-4]
                    - ((seq_t)6 ) * cache[i-5]
                    -          cache[i-6]
                    + ((seq_t)3 ) * cache[i-7]
                    -          cache[i-8];
        cache[i] = term;
    }

    return cache[(size_t)n];
}
