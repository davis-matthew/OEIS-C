/*
 * A132672  a(1)=1, a(n) = 8*a(n-1) if the minimal positive integer not yet in
 *          the sequence is greater than a(n-1), else a(n) = a(n-1) - 1.
 * https://oeis.org/A132672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(n)=8*a(n-1) if a(n-1)-1 is ≤0 or already used, else
 *            a(n)=a(n-1)-1. The sequence is a permutation of ℕ⁺
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */
#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

static seq_t *seq = NULL;            /* array of sequence values, 1‑based (seq[0] unused) */
static size_t seq_len = 0;           /* current number of stored values (seq_len-1 is last index) */
static size_t seq_cap = 0;           /* allocated capacity of seq */

static seq_t *used = NULL;           /* array of all values that have appeared so far */
static size_t used_len = 0;          /* number of used values stored */
static size_t used_cap = 0;          /* allocated capacity of used */

/* Grow an array of seq_t values so that it can hold at least 'cap' elements. */
static void ensure_seq_cap(size_t cap)
{
    if (cap <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq = tmp;
    seq_cap = new_cap;
}

/* Grow an array of seq_t values so that it can hold at least 'cap' elements. */
static void ensure_used_cap(size_t cap)
{
    if (cap <= used_cap) return;
    size_t new_cap = used_cap ? used_cap : 16;
    while (new_cap < cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(used, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    used = tmp;
    used_cap = new_cap;
}

/* Check whether a value has already appeared in the sequence. */
static bool used_contains(seq_t val)
{
    for (size_t j = 0; j < used_len; ++j)
        if (used[j] == val) return true;
    return false;
}

/* Record a value as having appeared. */
static void used_add(seq_t val)
{
    if (used_len == used_cap) ensure_used_cap(used_len + 1);
    used[used_len++] = val;
}

/* Compute the nth term of sequence A132672. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n=0; return 0 */

    size_t target = (size_t)n;

    /* lazy initialization */
    if (!seq) {
        ensure_seq_cap(16);
        seq_len = 2;                     /* indices 0..1 used */
        seq[0] = (seq_t)0;               /* dummy */
        seq[1] = (seq_t)1;               /* a(1) = 1 */
        ensure_used_cap(16);
        used_len = 1;
        used[0] = (seq_t)1;               /* value 1 is now used */
    }

    /* extend the sequence up to 'target' */
    for (size_t i = seq_len; i <= target; ++i) {
        seq_t prev = seq[i - 1];
        seq_t candidate = prev - (seq_t)1;   /* a(n-1)-1 */
        bool use_candidate = false;

        if (candidate > (seq_t)0) {
            use_candidate = !used_contains(candidate);
        }

        seq_t next;
        if (use_candidate) {
            next = candidate;
        } else {
            next = prev * (seq_t)8;
        }

        if (seq_len == seq_cap) ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = next;
        used_add(next);
    }

    return seq[target];
}
