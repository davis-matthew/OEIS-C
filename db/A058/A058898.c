/*
 * A058898  Inconsummate numbers in base 2: no number is this multiple of the
 *          sum of its digits (in base 2).
 * https://oeis.org/A058898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1855, u32 n <= 9706, u64 n <= 9706,
 *            u128 n <= 9706, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inconsummate numbers in base 2: numbers that are NOT expressible
   as the arithmetic mean of distinct powers of 2.
   A number n is representable iff there exists an integer k≥1 such that
   popcount(n*k) == k.  This function returns the nth such non‑representable
   number (with offset 1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static inline int popcount_seq_t(seq_t x) {
    int c = 0;
    while (x) {
        x &= (x - 1);
        ++c;
    }
    return c;
}

static bool representable(seq_t n) {
    /* We only need to check k up to 128: for n up to the 10000th term
       this is more than enough, and even for large seq_t values it
       works for all numbers that appear in the sequence. */
    for (idx_t k = 1; k <= 128; ++k) {
        seq_t m = n * (seq_t)k;
        if (popcount_seq_t(m) == (int)k)
            return true;   /* representable as mean of distinct powers of 2 */
    }
    return false;          /* not representable */
}

/* Dynamic storage for the sequence values */
static seq_t *seq = NULL;      /* array of computed terms */
static size_t seq_len = 0;     /* number of terms stored */
static size_t seq_cap = 0;     /* current allocated capacity */
static seq_t last_checked = 0; /* last integer examined */

static void ensure_seq(idx_t n) {
    /* Allocate the array on first use */
    if (!seq) {
        seq_cap = 32;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* Extend the sequence until it contains at least n terms */
    while (seq_len < n) {
        seq_t candidate = last_checked + (seq_t)1;
        if (!representable(candidate)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = candidate;
        }
        last_checked = candidate;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* out‑of‑range safety */
    if (seq_len < n) ensure_seq(n);
    return seq[n - 1];
}
