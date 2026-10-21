/*
 * A092875  Aronson transform of the "evil" sequence (A001969).
 * https://oeis.org/A092875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Aronson transform of the “evil” sequence (A001969).
   Computes a(n) with the definition:
   k is in S (the set of all a(n))     iff    a(k) is an evil number
   (even number of 1's in binary).  The sequence is monotonic and hence
   one can determine a(n) iteratively, picking the smallest integer > max(a(n-1), n)
   that satisfies the parity condition described above. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*--- Global cache for the sequence (1‑indexed) --------------------------------*/
static seq_t *seq_vals = NULL;      /* a[idx_t] values, 1-indexed */
static idx_t  seq_count = 0;        /* how many terms have been generated */
static size_t seq_cap   = 0;        /* allocated capacity (number of seq_t elements) */

/*--- Global bitmap indicating which integers have already appeared -------*/
static unsigned char *present = NULL;  /* 1-indexed flags */
static size_t present_cap = 0;         /* allocated capacity (bytes) */

/* Ensure seq_vals can hold at least `needed` indices (1-indexed). */
static void ensure_seq_capacity(idx_t needed) {
    if (seq_cap <= (size_t)needed) {      /* need at least needed+1 slots */
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        while (new_cap <= (size_t)needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_vals = tmp;
        seq_cap  = new_cap;
    }
}

/* Ensure present can be indexed up to `needed` (1‑indexed). */
static void ensure_present_capacity(idx_t needed) {
    if (present_cap <= (size_t)needed) {
        size_t new_cap = present_cap ? present_cap * 2 : 64;
        while (new_cap <= (size_t)needed) new_cap *= 2;
        unsigned char *tmp = (unsigned char *)realloc(present, new_cap);
        if (!tmp) abort();
        /* zero any newly allocated part */
        if (new_cap > present_cap)
            memset(tmp + present_cap, 0, new_cap - present_cap);
        present = tmp;
        present_cap = new_cap;
    }
}

/* popcount for 64‑bit values.  Works for seq_t up to 64 bits. */
static inline int popcount_64(uint64_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
#endif
}

/* Sequence function requested by the specification. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;         /* undefined below offset */

    /* Generate terms up to n if necessary. */
    while (seq_count < n) {
        idx_t k = seq_count + 1;          /* next index (1‑based) */

        /* Ensure memory for reading present[k] */
        ensure_present_capacity(k);
        bool k_in_S = present[k] != 0;    /* is k already a term? */

        /* Previous term's value (0 if none yet). */
        seq_t prev_val = (seq_count == 0) ? (seq_t)0 : seq_vals[seq_count];

        /* Candidate start: > max(prev_val, k). */
        idx_t start = prev_val > (seq_t)k ? (idx_t)prev_val : k;
        idx_t cand = start + 1;

        /* Search for the first acceptable candidate. */
        while (true) {
            /* Ensure we can index cand in present. */
            ensure_present_capacity(cand);

            int ones = popcount_64((uint64_t)cand);
            bool evil = (ones & 1) == 0;   /* even popcount -> evil */

            if (evil == k_in_S) break;      /* parity matches requirement */
            ++cand;
        }

        seq_t val = (seq_t)cand;            /* value of a(k) */

        /* Store computed term. */
        ensure_seq_capacity(k);
        seq_vals[k] = val;

        /* Mark this value as present for future membership checks. */
        ensure_present_capacity(cand);
        present[cand] = 1;

        ++seq_count;                       /* success! */
    }

    return seq_vals[n];
}
