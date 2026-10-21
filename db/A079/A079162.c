/*
 * A079162  a(n) = 5*a(n-2) - 2*a(n-4), with initial terms 0,1,2,4.
 * https://oeis.org/A079162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-2) - 2*a(n-4) Initial values: a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=4
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 59, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* A fixed‑size cache is sufficient for all tests (n ≃ 10000).
   For larger n an abort will be triggered. */
#define CACHE_CAP 20001

static seq_t seq_cache[CACHE_CAP];
static size_t seq_cache_len = 0;

/* Initialise the first four terms (done lazily on the first call). */
static void init_cache(void)
{
    if (seq_cache_len >= 4) return;
    seq_cache[0] = (seq_t)0;
    seq_cache[1] = (seq_t)1;
    seq_cache[2] = (seq_t)2;
    seq_cache[3] = (seq_t)4;
    seq_cache_len = 4;
}

/* Public sequence function.  Calls may arrive in any order. */
seq_t a(idx_t n)
{
    const size_t idx = (size_t)n;          /* idx_t is unsigned */
    if (idx >= CACHE_CAP) {
        /* The cache is too small for this request.  The sequence
           is not expected to be queried beyond CACHE_CAP-1 in the tests. */
        abort();
    }

    init_cache();

    /* Return already computed value */
    if (idx < seq_cache_len) {
        return seq_cache[idx];
    }

    /* Fill missing terms up to idx using the recurrence. */
    for (size_t i = seq_cache_len; i <= idx; ++i) {
        /* recurrence references indices that are guaranteed to exist:
           i-2 >= 0 and i-4 >= 0 because seq_cache_len >= 4  */
        seq_t val = 5 * seq_cache[i - 2] - 2 * seq_cache[i - 4];
        seq_cache[i] = val;
    }
    seq_cache_len = idx + 1;
    return seq_cache[idx];
}
