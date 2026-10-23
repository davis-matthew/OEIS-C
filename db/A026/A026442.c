/*
 * A026442  a(n) = least positive integer > a(n-1) and not a(i)*a(j)-1 for 1
 *          <= i <= j < n, n >= 2, with initial terms 3,4.
 * https://oeis.org/A026442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation of OEIS A026442:
 *
 *  a(1) = 3, a(2) = 4
 *  a(n) = least positive integer > a(n‑1) that is NOT of the
 *  form a(i)*a(j) – 1 for 1 ≤ i ≤ j < n.
 *
 *  The routine builds the sequence incrementally and keeps a bitset
 *  of forbidden values up to a fixed upper bound.  The bound is
 *  generous enough for n ≤ 10000 on all supported precisions.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Upper bound for forbidden values we track explicitly.
 * 4 194 304 = 1 << 22  bits  → 512 KB of memory. */
#define FORBID_BITS  ((size_t)1 << 22)
#define FORBID_WORDS (FORBID_BITS / 64)

/* Dynamic array holding the computed sequence values. */
static seq_t *seq = NULL;
static size_t seq_cap = 0;   /* capacity in elements */
static idx_t seq_len = 0;    /* number of values stored */

/* Bitset marking forbidden numbers < FORBID_BITS. */
static uint64_t forbid[FORBID_WORDS] = {0};

/* Set the bit corresponding to value v (0 ≤ v < FORBID_BITS). */
static inline void set_forbid(size_t v)
{
    forbid[v >> 6] |= (uint64_t)1 << (v & 63);
}

/* Test whether value v has been marked forbidden. */
static inline bool is_forbid(size_t v)
{
    return (forbid[v >> 6] >> (v & 63)) & 1;
}

/* Grow the sequence array to accommodate at least n elements. */
static void grow_seq(idx_t n)
{
    if ((size_t)n <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while ((size_t)n > new_cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failed */
    seq = tmp;
    seq_cap = new_cap;
}

/* Record all forbidden products involving the term at position idx
 * (1‑based).  The term a(idx) is already stored in seq[idx-1]. */
static void add_forbidden_for_index(idx_t idx)
{
    seq_t val_new = seq[idx - 1];
    for (idx_t i = 1; i <= idx; ++i)
    {
        seq_t val_i = seq[i - 1];
        seq_t prod = val_i * val_new - (seq_t)1;
        if (prod < (seq_t)FORBID_BITS)
            set_forbid((size_t)prod);
    }
}

/* Public function computing a(n). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Extend the precomputed part up to n if necessary. */
    if (seq_len < n)
    {
        for (idx_t next = seq_len + 1; next <= n; ++next)
        {
            grow_seq(next);

            /* Base cases. */
            if (next == 1)
                seq[0] = (seq_t)3;
            else if (next == 2)
                seq[1] = (seq_t)4;
            else
            {
                /* Candidate starts from previous term + 1. */
                seq_t cand = seq[next - 2] + (seq_t)1;
                while ((cand < (seq_t)FORBID_BITS) && is_forbid((size_t)cand))
                    ++cand;
                seq[next - 1] = cand;
            }

            /* Mark forbidden values that involve the newly added term. */
            add_forbidden_for_index(next);
        }
        seq_len = n;
    }

    return seq[n - 1];
}
