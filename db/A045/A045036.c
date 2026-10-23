/*
 * A045036  Numbers whose base-4 representation contains exactly four 0's and
 *          three 1's.
 * https://oeis.org/A045036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base-4 representation contains exactly four 0's
 *            and three 1's
 * Range:     u8 none, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* Dynamic array holding computed terms */
static idx_t  cache_size = 0;      /* Current number of stored terms   */
static idx_t  cache_cap  = 0;      /* Allocated capacity of the array   */
static int    last_length_processed = 6;   /* Largest length fully processed */

/* Ensure the cache has capacity at least 'needed' elements */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Recursive generation of length-L numbers with the required digit counts.
   `pos`      : current position (0 is most significant digit)
   `zeros`    : zero digits still to place (must stay ≥0)
   `ones`     : one digits still to place (must stay ≥0)
   `free`     : digits that may be 2 or 3 still to place (must stay ≥0)
   `val`      : value constructed so far
   `L`        : target length of the number
*/
static void generate_len_rec(int pos, int zeros, int ones, int free, seq_t val, int L)
{
    if (pos == L) {
        if (zeros == 0 && ones == 0 && free == 0) {
            if (cache_size >= cache_cap) grow_cache(cache_size + 1);
            cache[cache_size++] = val;
        }
        return;
    }

    /* Iterate digits in ascending order to preserve numeric order */
    for (int d = 0; d <= 3; ++d) {
        if (pos == 0 && d == 0) continue;         /* no leading zero */
        if (d == 0 && zeros == 0) continue;       /* no zero left */
        if (d == 1 && ones  == 0) continue;       /* no one left  */
        if (d > 1 && free == 0) continue;          /* no free digit left */

        int z = zeros, o = ones, f = free;
        if (d == 0)  --z;
        else if (d == 1) --o;
        else          --f;

        generate_len_rec(pos + 1, z, o, f, (val << 2) + (seq_t)d, L);
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n=0, safety check */

    /* Return cached result if already computed */
    if (n <= cache_size) return cache[n - 1];

    /* Generate additional terms until we have at least n terms */
    while (cache_size < n) {
        int L = ++last_length_processed;  /* next length to generate */
        generate_len_rec(0, 4, 3, L - 7, 0, L);
    }

    return cache[n - 1];
}
