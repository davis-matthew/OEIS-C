/*
 * A188598  Numbers k such that d(k-1) > d(k) > d(k+1) where d(k) is the
 *          number of divisors of k.
 * https://oeis.org/A188598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 4576, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that d(k-1) > d(k) > d(k+1)
 * (d(n) = number of divisors of n).  The function builds a sieve of
 * divisor counts up to a growing limit and stores all qualifying k
 * values in a dynamically resizable array.  Subsequent calls reuse
 * the cached data.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
/* helper functions for C++ builds: use new/delete */
static inline seq_t *new_array(size_t n) { return new seq_t[n]; }
static inline void delete_array(seq_t *p) { delete[] p; }
#else
/* helpers for C builds: use malloc/free */
static inline seq_t *new_array(size_t n) { return (seq_t *)malloc(n * sizeof(seq_t)); }
static inline void delete_array(seq_t *p) { free(p); }
#endif

/* static storage for divisor counts and the generated sequence */
static uint32_t *divcount = NULL;       /* divisor counts for indices 0 .. max_checked+1 */
static size_t max_checked = 0;          /* largest index for which divcount is valid */

static seq_t *seq   = NULL;             /* array of qualifying k values */
static size_t seq_len = 0;             /* number of elements stored */
static size_t seq_cap = 0;             /* allocated capacity of seq */

/* rebuild divisor counts and the sequence up to the given limit (inclusive) */
static void recompute(size_t limit)
{
    /* allocate divisor count array for indices 0 .. limit+1 */
    if (divcount) free(divcount);
    divcount = (uint32_t *)calloc(limit + 2, sizeof(uint32_t));
    if (!divcount) exit(1);

    /* sieve divisor counts */
    for (size_t i = 1; i <= limit; ++i) {
        for (size_t j = i; j <= limit; j += i)
            ++divcount[j];
    }

    /* rebuild the sequence array */
    delete_array(seq);
    seq_len = 0;
    seq_cap = (limit / 10) + 1;
    if (seq_cap < 10) seq_cap = 10;
    seq = new_array(seq_cap);
    if (!seq) exit(1);

    for (size_t k = 2; k + 1 <= limit; ++k) {
        if (divcount[k - 1] > divcount[k] && divcount[k] > divcount[k + 1]) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_t *tmp = new_array(seq_cap);
                if (!tmp) exit(1);
                memcpy(tmp, seq, seq_len * sizeof(seq_t));
                delete_array(seq);
                seq = tmp;
            }
            seq[seq_len++] = (seq_t)k;
        }
    }

    max_checked = limit;
}

/* ensure that at least n terms of the sequence have been computed */
static void ensure_sequence(idx_t n)
{
    if (seq_len >= (size_t)n) return;
    size_t new_limit = (max_checked == 0) ? 1000 : max_checked * 2;
    while (seq_len < (size_t)n) {
        recompute(new_limit);
        new_limit *= 2;
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n is 1‑based; return 0 for 0 */
    ensure_sequence(n);
    return seq[(size_t)n - 1];
}
