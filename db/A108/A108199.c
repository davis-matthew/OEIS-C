/*
 * A108199  a(n) contains the digits of the remainder of a(n)/a(n-1). Sequence
 *          starts with 2.
 * https://oeis.org/A108199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 158, u16 n <= 9592, u32 n <= 9592, u64 n <= 9592,
 *            u128 n <= 9592, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A108199: a(1)=2; for n>1, a(n) is the smallest integer >a(n-1) such that
   r = a(n) % a(n-1) is non‑zero and each digit of r (with multiplicity) occurs in a(n). */

#include <stdlib.h>
#include <stdbool.h>

/* Count decimal digits of v into d[10] (d must be zeroed before calling). */
static void count_digits(seq_t v, unsigned int d[10]) {
    if (v == 0) {
        d[0] = 1;
        return;
    }
    while (v != 0) {
        unsigned int digit = (unsigned int)(v % 10);
        d[digit]++; 
        v /= 10;
    }
}

/* Return true if all digits of rem appear in cand with at least the same multiplicity. */
static bool digits_subset(seq_t rem, seq_t cand) {
    unsigned int cnt_cand[10] = {0};
    unsigned int cnt_rem[10]  = {0};
    count_digits(cand, cnt_cand);
    count_digits(rem,  cnt_rem);
    for (int i = 0; i < 10; ++i)
        if (cnt_rem[i] > cnt_cand[i]) return false;
    return true;
}

/* Static storage for the computed sequence. */
static seq_t *seq_arr = NULL;   /* array of terms */
static size_t seq_cap = 0;      /* allocated capacity */
static idx_t  seq_len = 0;      /* number of terms stored */

/* Ensure terms up to index n (1‑based) are computed and stored. */
static void grow_to(idx_t n) {
    if (n <= seq_len) return;
    if (seq_cap == 0) {
        seq_cap = 16;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }
    while (seq_len < n) {
        seq_t prev;
        if (seq_len == 0) {          /* a(1) = 2 */
            prev = (seq_t)2;
            seq_arr[seq_len++] = prev;
            continue;
        }
        prev = seq_arr[seq_len - 1];
        seq_t cand = prev + (seq_t)1;
        for (;;) {
            seq_t r = cand % prev;
            if (r != 0 && digits_subset(r, cand)) {
                if (seq_len == seq_cap) {
                    seq_cap *= 2;
                    seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
                }
                seq_arr[seq_len++] = cand;
                break;
            }
            cand++;
        }
    }
}

/* Main sequence function, with 1‑based indexing. */
seq_t a(idx_t n) {
    if (n == 0) return 0;      /* undefined for n=0, but safe */
    grow_to(n);
    return seq_arr[n - 1];
}
