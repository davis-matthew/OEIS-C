/*
 * A130457  Triangle, read by rows of 3n+1 terms, where row n+1 is generated
 *          by taking partial sums of row n and then appending 2 zeros
 *          followed by the final term in the partial sums of row n, for n>=0,
 *          with T(0,0)=1.
 * https://oeis.org/A130457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A130457:  Flattened triangle generated recursively.
   Triangle row n has 3n+1 terms.
   T(0,0)=1.
   For n≥0:
      T(n+1,k) = Σ_{i=0}^{k} T(n,i)  for 0≤k<3n+1
      T(n+1,3n+1)=T(n+1,3n+2)=0
      T(n+1,3n+3) = Σ_{i=0}^{3n} T(n,i)
   The sequence a(n) is the row‑wise flattening of the triangle,
   starting with a(0)=1. */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

static seq_t *cache      = NULL;   /* flattened sequence up to current size */
static size_t cache_size = 0;      /* number of elements stored in cache  */
static size_t cache_cap   = 0;      /* allocated capacity of cache          */

static seq_t *prev_row  = NULL;    /* array holding the current row        */
static idx_t  prev_len  = 0;       /* length of prev_row                   */
static idx_t  current_row = 0;     /* number of the row that prev_row is  */

/* static flag */
static int initialized = 0;

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = alloc_seq_array(new_cap);
    /* copy existing elements */
    for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
    /* for safety in arbitrary‑precision builds we do not free the old
       memory; the host program will terminate soon enough. */
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute the next row after the current one and append its elements
   to the flattened cache. */
static void generate_next_row(void)
{
    idx_t next_row = current_row + 1;
    idx_t new_len  = 3 * next_row + 1;      /* length of the new row   */

    /* allocate array for the new row */
    seq_t *curr = alloc_seq_array(new_len);

    /* partial sums of previous row */
    seq_t sum = 0;
    for (idx_t i = 0; i < prev_len; ++i) {
        sum += prev_row[i];
        curr[i] = sum;
    }

    /* append two zeros */
    curr[prev_len]     = 0;
    curr[prev_len + 1] = 0;

    /* append the last partial sum again */
    curr[prev_len + 2] = sum;

    /* append the entire new row to the cache */
    ensure_cache_capacity(cache_size + new_len);
    for (idx_t i = 0; i < new_len; ++i) {
        cache[cache_size++] = curr[i];
    }

    /* advance to the next row */
    prev_row  = curr;
    prev_len  = new_len;
    current_row = next_row;
}

seq_t a(idx_t n)
{
    /* first time: initialise structures */
    if (!initialized) {
        cache_cap = 16;
        cache = alloc_seq_array(cache_cap);
        cache[0] = 1;
        cache_size = 1;

        prev_len = 1;
        prev_row = alloc_seq_array(1);
        prev_row[0] = 1;
        current_row = 0;

        initialized = 1;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* otherwise generate rows until we reach the desired index */
    while (cache_size <= n)
        generate_next_row();

    return cache[n];
}
