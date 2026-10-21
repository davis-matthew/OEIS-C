/*
 * A054082  Permutation of N: a(1)=2, a(2)=1 and for each k >= 2, let
 *          p(k)=least natural number not already an a(i), q(k)=p(k)+k-1,
 *          a(2k-1)=q(k), a(2k)=p(k).
 * https://oeis.org/A054082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A054082 – Permutation of N
   a(1)=2, a(2)=1.
   For each k ≥ 2:
     p(k) = least natural number not already appearing in a(),
     q(k) = p(k) + k – 1,
     a(2k‑1) = q(k),
     a(2k)   = p(k).
   This code builds the sequence up to the requested index, using
   dynamic arrays and a bitmap of seen natural numbers.  The state is
   memoised so that calls in any order are fast.  It works for all
   supported seq_t / idx_t combinations. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Static memoisation state */
static seq_t *seq        = NULL; /* 1‑based sequence values */
static size_t seq_cap    = 0;    /* capacity of seq array */

static bool *seen        = NULL; /* seen[n] == true if n has appeared */
static size_t seen_cap   = 0;    /* capacity of seen array */

static idx_t memo_n      = 0;    /* largest index that has been computed */
static uint64_t smallest_missing = 1; /* current smallest missing natural */

/* Ensure that 'seen' can index at least number 'x' (x ≥ 1) */
static void ensure_seen_capacity(uint64_t x)
{
    if (x >= seen_cap) {
        size_t new_cap = seen_cap ? seen_cap * 2 : 128;
        while (new_cap <= x)
            new_cap <<= 1;
        bool *tmp = (bool *)realloc(seen, new_cap * sizeof(bool));
        if (!tmp)
            abort(); /* allocation failed */
        /* initialise new part to false */
        memset(tmp + seen_cap, 0, (new_cap - seen_cap) * sizeof(bool));
        seen      = tmp;
        seen_cap  = new_cap;
    }
}

/* Ensure that 'seq' can index at least index 'i' (i ≥ 1) */
static void ensure_seq_capacity(idx_t i)
{
    if ((size_t)i >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        while (new_cap <= (size_t)i)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp)
            abort(); /* allocation failed */
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* Advance smallest_missing to the next unseen natural number */
static void bump_smallest_missing(void)
{
    while (true) {
        if ((uint64_t)smallest_missing >= seen_cap)
            ensure_seen_capacity(smallest_missing);
        if (!seen[smallest_missing])
            break;
        ++smallest_missing;
    }
}

/* Initialise memoisation data structures and first two terms */
static void init_sequence(void)
{
    if (seq != NULL)
        return; /* already initialised */

    seq_cap = 8;
    seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    if (!seq)
        abort();

    seen_cap = 128;
    seen = (bool *)calloc(seen_cap, sizeof(bool));
    if (!seen)
        abort();

    /* a(1) = 2, a(2) = 1 */
    ensure_seq_capacity(2);
    seq[1] = (seq_t)2;
    seq[2] = (seq_t)1;

    /* Mark 1 and 2 as seen */
    ensure_seen_capacity(2);
    seen[1] = true;
    seen[2] = true;

    bump_smallest_missing(); /* should become 3 */

    memo_n = 2;
}

/* Main function requested */
seq_t a(idx_t n)
{
    init_sequence();

    if (n <= memo_n)
        return seq[(size_t)n];

    /* Next k to process */
    idx_t k = (memo_n / 2) + 1;   /* for memo_n==2 => k==2 */

    while (memo_n < n) {
        /* p(k) = current smallest missing natural number */
        uint64_t pk = smallest_missing;
        seq_t p = (seq_t)pk;

        /* q(k) = p(k) + k – 1 */
        seq_t q = p + (seq_t)(k - 1);

        /* indices for sequence */
        idx_t idx_odd  = 2 * k - 1; /* a(2k-1) = q */
        idx_t idx_even = 2 * k;     /* a(2k)   = p */

        /* ensure storage */
        ensure_seq_capacity(idx_even);
        ensure_seen_capacity(pk);
        ensure_seen_capacity((uint64_t)q);

        /* store values */
        seq[idx_odd]  = q;
        seq[idx_even] = p;

        /* mark as seen */
        seen[pk] = true;
        seen[(uint64_t)q] = true;

        /* update smallest_missing */
        bump_smallest_missing();

        memo_n = idx_even;
        ++k;
    }

    return seq[(size_t)n];
}
