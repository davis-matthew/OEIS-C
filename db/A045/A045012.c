/*
 * A045012  Numbers whose base-3 representation contains exactly four 0's and
 *          three 2's.
 * https://oeis.org/A045012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = the nth integer whose base‑3 representation
          contains exactly four 0’s and three 2’s. 
*/

#include <stdint.h>
#include <stdbool.h>

/* Maximum number of terms we may need (well above 10 000). */
#define MAX_CACHE 200000

/* Cached values of the sequence. */
static seq_t cache[MAX_CACHE];
static idx_t cache_count = 0;   /* how many terms are already cached */
static seq_t current_candidate = 0;  /* last integer examined */

/* Check whether a base‑3 number has four 0’s and three 2’s. */
static inline bool qualifies(seq_t x)
{
    int zeros = 0, twos = 0;
    seq_t t = x;

    while (t > (seq_t)0) {
        unsigned d = (unsigned)(t % (seq_t)3);
        if (d == 0)
            ++zeros;
        else if (d == 2)
            ++twos;

        /* Early exit if counts already exceed limits. */
        if (zeros > 4 || twos > 3)
            return false;

        t /= (seq_t)3;
    }
    return zeros == 4 && twos == 3;
}

/* Make sure at least 'need' terms are cached. */
static void ensure_cache(idx_t need)
{
    while (cache_count < need) {
        ++current_candidate;
        if (qualifies(current_candidate)) {
            if (cache_count < MAX_CACHE)
                cache[cache_count++] = current_candidate;
            else
                return;   /* should not happen for requested n */
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;
    ensure_cache(n);
    return cache[n - 1];
}
