/*
 * A100208  Minimal permutation of the natural numbers such that the sum of
 *          squares of two consecutive terms is a prime.
 * https://oeis.org/A100208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n>1, a(n)=the smallest m not seen before such that
 *            m^2 + a(n-1)^2 is prime
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/isprime_big.h"

static seq_t   *seq   = NULL;      /* cached sequence values        */
static unsigned char *seen = NULL;  /* bitmap of numbers already used */
static idx_t seq_len = 0;          /* capacity of seq array          */
static idx_t seq_used = 0;         /* number of terms computed       */
static idx_t seen_max = 0;         /* largest index seen in seen[]   */

/* Ensure that the bitmap can index up to 'max_value'.  */
static void
ensure_seen(idx_t max_value)
{
    if (max_value <= seen_max)
        return;
    idx_t new_max = max_value;
    size_t new_size = (size_t)(new_max + 1);
    unsigned char *tmp = (unsigned char *)realloc(seen, new_size);
    if (!tmp)
        exit(EXIT_FAILURE);
    memset(tmp + seen_max + 1, 0, (size_t)(new_max - seen_max));
    seen = tmp;
    seen_max = new_max;
}

/* Ensure the sequence array has at least 'min_capacity' slots.  */
static void
ensure_seq(idx_t min_capacity)
{
    if (seq_len >= min_capacity)
        return;
    idx_t new_len = seq_len ? seq_len * 2 : 8;
    if (new_len < min_capacity)
        new_len = min_capacity;
    seq_t *tmp = (seq_t *)realloc(seq, (size_t)new_len * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    seq = tmp;
    seq_len = new_len;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but return 0 safely */

    /* Lazy initialization on first call.  */
    if (seq_used == 0) {
        ensure_seq(1);
        seq[0] = (seq_t)1;
        ensure_seen(1);
        seen[1] = 1;
        seq_used = 1;
    }

    /* Grow the cached sequence up to the requested index.  */
    while (seq_used < n) {
        seq_t prev = seq[seq_used - 1];
        idx_t m = 1;
        for (;;) {
            ensure_seen(m);
            if (seen[m]) {
                ++m;
                continue;
            }
            seq_t mm = (seq_t)m;
            seq_t sum_sq = mm * mm + prev * prev;
            if (isprime_big(sum_sq))
                break;
            ++m;
        }
        /* Store the new term.  */
        ensure_seq(seq_used + 1);
        seq[seq_used] = (seq_t)m;
        seen[m] = 1;
        ++seq_used;
    }
    return seq[n - 1];
}
