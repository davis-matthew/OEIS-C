/*
 * A025635  Numbers of form 9^i*10^j, with i, j >= 0.
 * https://oeis.org/A025635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest positive number of the form 9^i·10^j (i,j ≥ 0)
 *            that appears as the n-th element in the sequence sorted in
 *            ascending order
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 57, u64 n <= 214, u128 n <= 817,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *g_cache = NULL;      /* cached terms, zero‑based          */
static size_t   g_cache_cap = 0;   /* allocation capacity               */
static idx_t   g_cache_len = 0;    /* number of terms cached            */
static idx_t   g_i9 = 0;           /* pointer for multiplication by 9   */
static idx_t   g_i10 = 0;          /* pointer for multiplication by 10 */

seq_t a(idx_t n)
{
    /* offset is 1; treat n = 0 as a guard */
    if (n == 0)
        return 0;

    idx_t target = n - 1;          /* zero‑based index                 */

    /* initialise cache if empty */
    if (g_cache_len == 0) {
        g_cache_cap = 64;
        g_cache = (seq_t *)malloc(sizeof(seq_t) * g_cache_cap);
        if (g_cache == NULL)
            abort();
        g_cache[0] = (seq_t)1;
        g_cache_len = 1;
        g_i9 = 0;
        g_i10 = 0;
    }

    /* extend cache until it contains the target element */
    while (g_cache_len <= target) {
        /* expand allocation if necessary */
        if (g_cache_len == g_cache_cap) {
            size_t new_cap = g_cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(g_cache, sizeof(seq_t) * new_cap);
            if (tmp == NULL)
                abort();
            g_cache = tmp;
            g_cache_cap = new_cap;
        }

        /* compute next candidates: 9 * cache[i9] and 10 * cache[i10] */
        seq_t next9  = g_cache[g_i9] * (seq_t)9;
        seq_t next10 = g_cache[g_i10] * (seq_t)10;

        /* take the smaller one, handling equal values */
        seq_t next;
        if (next9 < next10) {
            next = next9;
        } else if (next10 < next9) {
            next = next10;
        } else { /* equality: both candidates are the same */
            next = next9; /* or next10 */
        }

        /* append the selected value */
        g_cache[g_cache_len++] = next;

        /* advance pointers for the used candidates */
        if ((next9 <= next10) && (next==next9))
            ++g_i9;
        if ((next10 <= next9) && (next==next10))
            ++g_i10;
    }

    return g_cache[target];
}
