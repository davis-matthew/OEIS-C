/*
 * A161790  The positive integer n is included if 1 is the largest integer of
 *          the form {2^k - 1} to divide n.
 * https://oeis.org/A161790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 139, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer that is not divisible by any Mersenne number >1 (2^k‑1 for k>1). */

#include <stdbool.h>
#include <stdlib.h>

/* Forward declaration appears in the build header; do NOT redefine these types. */
/* typedef ... seq_t; */
/* typedef ... idx_t; */

/* Cached sequence terms. */
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

/* Copied highest number that has been examined. */
static idx_t last_checked = 0;

/* Pre‑computed list of Mersenne numbers (2^k‑1, k>1). */
static idx_t *mersenne_arr = NULL;
static size_t mersenne_cap = 0;
static size_t mersenne_len = 0;
static idx_t mersenne_last   = 0;   /* largest Mersenne value stored */

/* Ensure *mersenne_arr* contains all Mersenne numbers <= *limit* (excluding 1). */
static void ensure_mersenne_up_to(idx_t limit)
{
    if (limit <= mersenne_last) return;   /* already covered */

    /* Initialise array on first use. */
    if (mersenne_cap == 0) {
        mersenne_cap = 16;
        mersenne_arr = (idx_t*)malloc(mersenne_cap * sizeof(idx_t));
    }

    /* Start from 3 if the list is empty. */
    if (mersenne_len == 0) {
        mersenne_arr[0] = 3;
        mersenne_len   = 1;
        mersenne_last  = 3;
    }

    /* Generate successive Mersenne numbers: 2^k – 1.  We use a 128‑bit
       intermediate to avoid overflow when shifting. */
    while (1) {
        if (mersenne_last > (limit - 1) / 2) break;   /* next would exceed limit */

        unsigned __int128 nxt = ((unsigned __int128)mersenne_last << 1) + 1;
        if (nxt > limit) break;

        if (mersenne_len == mersenne_cap) {
            mersenne_cap <<= 1;
            mersenne_arr = (idx_t*)realloc(mersenne_arr, mersenne_cap * sizeof(idx_t));
        }
        mersenne_arr[mersenne_len++] = (idx_t)nxt;
        mersenne_last = (idx_t)nxt;
    }
}

/* Return true iff *num* has no Mersenne divisor (greater than 1). */
static bool is_good(idx_t num)
{
    for (size_t i = 0; i < mersenne_len; ++i) {
        if (num % mersenne_arr[i] == 0) return false;
    }
    return true;
}

/* Grow the sequence array so that it contains at least *n* terms. */
static void ensure_sequence_up_to(idx_t n)
{
    /* Continue generating until the array has n elements. */
    while (seq_len < (size_t)n) {
        idx_t candidate = last_checked + 1;

        /* Find the next number that is not divisible by any Mersenne. */
        while (1) {
            ensure_mersenne_up_to(candidate);
            if (is_good(candidate)) break;
            ++candidate;
        }

        /* Store the candidate. */
        if (seq_len == seq_cap) {
            size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
            seq_arr = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }
        seq_arr[seq_len++] = (seq_t)candidate;
        last_checked = candidate;
    }
}

/* Public function as required. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for the problem, but safe guard */
    ensure_sequence_up_to(n);
    return seq_arr[n - 1];
}
