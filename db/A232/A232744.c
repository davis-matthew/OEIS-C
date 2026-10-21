/*
 * A232744  Numbers k for which the largest m such that m! divides k is odd.
 * https://oeis.org/A232744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 162, u16 n <= 9558, u32 n <= 9558, u64 n <= 9558,
 *            u128 n <= 9558, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: sequence of integers k such that the largest m with m! | k
   is odd.  The function generates terms on demand by testing integers
   in order and caching them in a static array.  This works for both
   fixed‑width unsigned types and arbitrary‑precision integer classes. */

#include <stdlib.h>

static seq_t *seq = NULL;          /* cached terms               */
static size_t seq_len = 0;         /* number of cached terms     */
static size_t seq_cap = 0;         /* array capacity             */
static idx_t   cur_k   = 1;        /* next integer to test       */

/* Ensure the cache array has at least *needed* capacity.  Reallocate
   with doubling strategy; use `new` in C++ and `realloc` in C. */
static void seq_ensure_cap(size_t needed)
{
    if (needed <= seq_cap) return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;

#ifdef __cplusplus
    seq_t *new_seq = new seq_t[new_cap];
    for (size_t i = 0; i < seq_len; ++i) new_seq[i] = seq[i];
    delete[] seq;
    seq = new_seq;
#else
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) exit(EXIT_FAILURE);   /* fatal if memory cannot be allocated */
#endif
    seq_cap = new_cap;
}

/* Return the n-th term (n ≥ 1).  The function is idempotent and can
   be called in any order because it lazily generates and caches terms.
   Cache growth is O(n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n < 1           */

    while (seq_len < (size_t)n) {
        seq_t k = (seq_t)cur_k;

        /* Find the largest m such that m! divides k. */
        seq_t f = (seq_t)1;  /* f = m!   (starts with 0! = 1)      */
        idx_t  m = 0;        /* current m                         */

        while (1) {
            /* If k is divisible by current factorial. */
            if (k % f == (seq_t)0) {
                ++m;

                /* Prepare next factorial f *= (m+1).  Stop if it would
                   exceed k or overflow the representation. */
                if (f > k / ( (seq_t)(m + 1) )) break;
                f *= (seq_t)(m + 1);
            } else {
                break;
            }
        }

        /* Include k if m is odd. */
        if ((m & 1ULL) != 0ULL) {
            seq_ensure_cap(seq_len + 1);
            seq[seq_len++] = k;
        }

        ++cur_k;
    }

    return seq[(size_t)n - 1];
}
