/*
 * A081840  a(1)=0, a(n)=a(n-1)+3 if n is already in the sequence,
 *          a(n)=a(n-1)+4 otherwise.
 * https://oeis.org/A081840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+3 if n is already in the sequence, else +4 Initial
 *            term: a(1) = 0 Implements memoised O(n) computation and keeps
 *            all seen sequence values (even those larger than the current
 *            index) to allow membership tests for future queries
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stddef.h>

static seq_t *seq_vals  = NULL;   /* sequence values, a(1)=seq_vals[0]          */
static size_t seq_len   = 0;       /* number of terms stored                     */
static size_t seq_cap   = 0;       /* allocated capacity for seq_vals            */

static bool *seen      = NULL;     /* bitmap: seen[v] == true iff v is a seq value */
static size_t seen_cap = 0;        /* allocated capacity for seen                 */

/* Grow seq_vals to at least min_cap elements */
static void ensure_seq_cap(size_t min_cap)
{
    if (min_cap <= seq_cap)
        return;
    seq_vals = (seq_t *)realloc(seq_vals, min_cap * sizeof(seq_t));
    assert(seq_vals != NULL);
    seq_cap = min_cap;
}

/* Grow seen to at least min_cap elements, zero‑initialising new part */
static void ensure_seen_cap(size_t min_cap)
{
    if (min_cap <= seen_cap)
        return;
    size_t new_cap = seen_cap ? seen_cap : 16;
    while (new_cap < min_cap)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(seen, new_cap * sizeof(bool));
    assert(tmp != NULL);
    if (new_cap > seen_cap)
        /* zero the newly allocated chunk */
        memset(tmp + seen_cap, 0, (new_cap - seen_cap) * sizeof(bool));
    seen      = tmp;
    seen_cap  = new_cap;
}

/* Main function ----------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)   /* safety for out‑of‑bounds callers */
        return (seq_t)0;

    size_t N = (size_t)n;      /* zero‑based index handling */

    /* return cached term if already computed */
    if (N <= seq_len)
        return seq_vals[N - 1];

    /* Ensure buffer for future membership tests up to current N. */
    ensure_seen_cap(N + 1);

    /* Grow sequence storage to hold at least N terms */
    ensure_seq_cap(N);

    /* Compute terms from seq_len+1 up to N */
    for (size_t i = seq_len + 1; i <= N; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)0;
        } else {
            seq_t prev = seq_vals[i - 2];
            bool inc3  = seen[i];               /* check if i is already a sequence value */
            val = prev + (inc3 ? (seq_t)3 : (seq_t)4);
        }
        seq_vals[i - 1] = val;

        /* Record this value for future membership checks.
           We need it even if val > N, because later indices may query it. */
        if ((size_t)val >= seen_cap)
            ensure_seen_cap((size_t)val + 1);
        seen[(size_t)val] = true;
    }

    seq_len = N;
    return seq_vals[N - 1];
}
