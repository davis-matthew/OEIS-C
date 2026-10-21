/*
 * A085947  a(1) = 1, a(2) = 2 and a(n) = smallest number not included earlier
 *          that divides the sum of the two previous terms.
 * https://oeis.org/A085947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS recurrence:
   a(1) = 1, a(2) = 2;
   for n>=3: a(n) is the smallest positive integer not already used that divides
   a(n-1)+a(n-2). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t *seq = NULL;          /* array of sequence values, 0‑based */
static size_t seq_len = 0;         /* number of computed terms */
static size_t seq_cap = 0;         /* allocated capacity for seq */

static uint8_t *used = NULL;       /* bit array: used[x] == 1 if x already used */
static size_t used_cap = 0;        /* allocated capacity for used */

/* Grow the sequence array if needed */
static void grow_seq(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Grow the used array to allow indexing at least val */
static void ensure_used(seq_t val)
{
    size_t needed = (size_t)val + 1;   /* +1 because 0 is unused */
    if (needed <= used_cap)
        return;
    size_t new_cap = used_cap ? used_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
    /* Zero the newly allocated part */
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* Find the smallest unused divisor of sum */
static seq_t smallest_unused_divisor(seq_t sum)
{
    seq_t best = 0;
    /* Iterate divisors d from 1 up to sqrt(sum) */
    for (idx_t d = 1; (seq_t)d <= sum / (seq_t)d; ++d)
    {
        if (sum % (seq_t)d != 0)
            continue;

        /* Candidate d */
        seq_t cand = (seq_t)d;
        ensure_used(cand);
        if (!used[(size_t)cand] && (best == 0 || cand < best))
            best = cand;

        /* Candidate sum/d */
        seq_t cand2 = sum / (seq_t)d;
        if (cand2 == cand)        /* avoid double work for perfect squares */
            continue;
        ensure_used(cand2);
        if (!used[(size_t)cand2] && (best == 0 || cand2 < best))
            best = cand2;
    }
    /* best must be found because 1 always divides, but 1 is used already */
    return best;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1, but return 0 */

    size_t idx = (size_t)n - 1;

    /* Initialize on first call */
    if (seq_len == 0)
    {
        grow_seq(16);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        seq_len = 2;
        ensure_used(1);
        ensure_used(2);
        used[(size_t)1] = 1;
        used[(size_t)2] = 1;
    }

    /* Compute terms up to the requested index */
    while (seq_len <= idx)
    {
        seq_t sum = seq[seq_len - 1] + seq[seq_len - 2];
        seq_t val = smallest_unused_divisor(sum);

        grow_seq(seq_len + 1);
        seq[seq_len] = val;
        ensure_used(val);
        used[(size_t)val] = 1;
        ++seq_len;
    }

    return seq[idx];
}
