/*
 * A086289  Total number of prime factors of 7-smooth numbers.
 * https://oeis.org/A086289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega( the n-th 7‑smooth number ), where 7‑smooth
 *            numbers are the integers with all prime factors ≤ 7. The
 *            implementation builds the sequence by the standard multi‑pointer
 *            method for Hamming numbers (2‑,3‑,5‑,7‑smooth)
 * Range:     u8 n <= 76, u16 n <= 1066, u32 n <= 8353, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static tables holding the 7‑smooth numbers and their Ω values */
static seq_t *smooth_vals  = NULL;   /* 7‑smooth numbers, indexed from 1 */
static seq_t *smooth_omega = NULL;   /* Ω of each smooth number */
static size_t size_cap    = 0;        /* allocated capacity (≥ number of terms) */
static idx_t  ptr2 = 1, ptr3 = 1, ptr5 = 1, ptr7 = 1; /* current indices for 2,3,5,7 */
static size_t term_count  = 0;        /* last index that has been computed */

/* Ensure that tables contain at least the first n terms.  The routine
   automatically grows the tables as needed and generates any missing
   values. */
static void ensure_terms(idx_t n)
{
    if (n < 1) return;                     /* nothing to do for n=0 */

    /* Grow the capacity if required */
    if (size_cap < (size_t)n) {
        size_t new_cap = size_cap ? size_cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;

        smooth_vals  = (seq_t *)realloc(smooth_vals,  (new_cap + 1) * sizeof(seq_t));
        smooth_omega = (seq_t *)realloc(smooth_omega, (new_cap + 1) * sizeof(seq_t));
        size_cap = new_cap;
    }

    /* Initialise the first element if this is the very first call. */
    if (term_count == 0) {
        smooth_vals[1]   = (seq_t)1;
        smooth_omega[1]  = (seq_t)0;
        ptr2 = ptr3 = ptr5 = ptr7 = 1;
        term_count = 1;
    }

    /* Generate the missing terms up to n */
    for (size_t cur = term_count + 1; cur <= (size_t)n; ++cur) {
        seq_t v2 = smooth_vals[ptr2] * (seq_t)2;
        seq_t v3 = smooth_vals[ptr3] * (seq_t)3;
        seq_t v5 = smooth_vals[ptr5] * (seq_t)5;
        seq_t v7 = smooth_vals[ptr7] * (seq_t)7;

        seq_t next = v2;
        if (v3 < next) next = v3;
        if (v5 < next) next = v5;
        if (v7 < next) next = v7;

        smooth_vals[cur] = next;

        /* The Ω value for the new term is Ω of the predecessor plus one. */
        if (next == v2) { smooth_omega[cur] = smooth_omega[ptr2] + 1; ++ptr2; }
        if (next == v3) { smooth_omega[cur] = smooth_omega[ptr3] + 1; ++ptr3; }
        if (next == v5) { smooth_omega[cur] = smooth_omega[ptr5] + 1; ++ptr5; }
        if (next == v7) { smooth_omega[cur] = smooth_omega[ptr7] + 1; ++ptr7; }
    }

    term_count = (size_t)n;
}

/* Public function required by the harness.  It returns a(n). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined below offset, return 0 */
    ensure_terms(n);
    return smooth_omega[(size_t)n];
}
