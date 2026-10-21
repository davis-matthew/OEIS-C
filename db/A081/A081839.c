/*
 * A081839  a(1)=0, a(n)=a(n-1)+4 if n is already in the sequence,
 *          a(n)=a(n-1)+5 otherwise.
 * https://oeis.org/A081839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1, a(n)=a(n-1)+4 if n is already a term, otherwise
 *            a(n)=a(n-1)+5
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static memoisation cache */
static seq_t *seq_cache = NULL;
static idx_t seq_len  = 0;    /* highest index cached (inclusive) */
static size_t seq_cap = 0;    /* allocated capacity */

static void ensure_cache(idx_t need)
{
    if (need <= seq_len) return;

    /* size needed for indices up to 'need' (1‑based) */
    size_t size_needed = (size_t)need + 1;
    if (size_needed > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap : 4;
        while (new_cap < size_needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failure */
        seq_cache = tmp;
        seq_cap   = new_cap;
    }

    /* initialise first term if cache is empty */
    if (seq_len == 0) {
        seq_cache[1] = (seq_t)0; /* a(1) = 0 */
        seq_len = 1;
    }

    /* compute missing terms */
    for (idx_t i = seq_len + 1; i <= need; ++i) {
        /* binary search to test if i is already a term */
        bool present = false;
        idx_t lo = 1, hi = i - 1;
        seq_t target = (seq_t)i;
        while (lo <= hi) {
            idx_t mid = (lo + hi) >> 1;
            seq_t val = seq_cache[mid];
            if (val == target) {
                present = true;
                break;
            }
            if (val < target)
                lo = mid + 1;
            else
                hi = mid - 1;
        }

        seq_t prev = seq_cache[i - 1];
        seq_cache[i] = prev + (present ? (seq_t)4 : (seq_t)5);
    }
    seq_len = need;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* harmless for out‑of‑range */
    ensure_cache(n);
    return seq_cache[n];
}
