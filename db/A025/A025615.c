/*
 * A025615  Numbers of form 3^i*8^j, with i, j >= 0.
 * https://oeis.org/A025615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth smallest positive integer of the form 3^i * 8^j
 *            (i, j >= 0). The sequence is obtained by sorting all products
 *            3^i * 8^j in increasing order. The algorithm is the classic
 *            two‑pointer “humble‑numbers” method applied to the multipliers 3
 *            and 8
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 124, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for size_t, NULL */
#include <stddef.h>
#include <stdint.h>

/* -------------------------------------------------------------------
   Helper: allocation of sequences for arbitrary‑precision seq_t.
   The build supplies this function; it returns a fresh array of
   seq_t elements suitable for use in both C and C++.
   ------------------------------------------------------------------- */
#include "../utils/alloc_seq_array.h"   /* provides seq_t *alloc_seq_array(size_t) */
#include "../utils/seq_free_array.h"    /* provides void seq_free_array(seq_t *) */

/* -------------------------------------------------------------------
   Static cache state – shared by all invocations of a().
   ------------------------------------------------------------------- */
static seq_t *seq_cache = NULL;   /* array of values (1‑indexed)          */
static size_t seq_capacity = 0;   /* allocated capacity (excluding index 0) */
static size_t seq_len = 0;        /* number of values known (>=1)          */
static idx_t  ptr3 = 0;           /* index for next multiple of 3         */
static idx_t  ptr8 = 0;           /* index for next multiple of 8         */
static seq_t  next3 = (seq_t)0;   /* next candidate from the 3‑pointer    */
static seq_t  next8 = (seq_t)0;   /* next candidate from the 8‑pointer    */

/* -------------------------------------------------------------------
   Ensure that seq_cache has room for at least ‘need’ elements
   (including index 0).  Grow the cache if necessary.
   ------------------------------------------------------------------- */
static void ensure_capacity(size_t need)
{
    if (need <= seq_capacity)
        return;

    size_t new_cap = seq_capacity ? seq_capacity * 2 : 4;
    if (new_cap < need)
        new_cap = need;

    /* Allocate new array (1‑indexed). */
    seq_t *new_cache = alloc_seq_array(new_cap + 1);
    /* Copy existing values. */
    for (idx_t i = 1; i <= (idx_t)seq_len; ++i)
        new_cache[i] = seq_cache[i];
    /* Free old array. */
    if (seq_cache != NULL)
        seq_free_array(seq_cache);
    /* Commit new cache. */
    seq_cache = new_cache;
    seq_capacity = new_cap;
}

/* -------------------------------------------------------------------
   Initialise the cache on the first call to a().
   ------------------------------------------------------------------- */
static void initialise(void)
{
    ensure_capacity(4);            /* start capacity */
    seq_cache[1] = (seq_t)1;       /* first term */
    seq_len = 1;

    /* set up the two pointers for generating multiples of 3 and 8 */
    ptr3 = 1;
    ptr8 = 1;
    next3 = seq_cache[1] * (seq_t)3;
    next8 = seq_cache[1] * (seq_t)8;
}

/* -------------------------------------------------------------------
   Public function required by the framework.
   ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (seq_cache == NULL)
        initialise();              /* first call – initialise cache */

    /* Generate terms until we reach the desired index n. */
    while (seq_len < (size_t)n)
    {
        ensure_capacity(seq_len + 1);

        /* Choose the smallest next candidate. */
        seq_t val;
        if (next3 < next8)
            val = next3;
        else if (next8 < next3)
            val = next8;
        else
            val = next3;           /* both equal – choose any */

        /* Store the new value. */
        seq_cache[seq_len + 1] = val;

        /* Advance any pointers that produced this value. */
        if (val == next3)
        {
            ++ptr3;
            next3 = seq_cache[(size_t)ptr3] * (seq_t)3;
        }
        if (val == next8)
        {
            ++ptr8;
            next8 = seq_cache[(size_t)ptr8] * (seq_t)8;
        }

        ++seq_len;
    }

    return seq_cache[(size_t)n];
}
