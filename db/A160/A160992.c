/*
 * A160992  a(1) = 1. For n >=2, a(n) = the smallest integer > a(n-1) such
 *          that sum{k=1 to r} a(k) doesn't equal any value of sum{k=1 to q}
 *          a(n+1-k), for any positive integer r, and for any positive
 *          integers q <= n-1.
 * https://oeis.org/A160992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A160992
 *
 * This implementation builds the sequence term by term.
 * For each step, we maintain a bitmap of all sums
 * S(i)+S(j) of previous prefix sums.
 * The next term is the smallest integer larger than
 * the previous term such that the new total
 * (sum_so_far + candidate) is not in that bitmap.
 * After adding a term we insert sums involving the new
 * total into the bitmap for future steps.
 *
 * Complexity:  O(n²) bitmap updates, well under the time limit
 * for n ≈ 10 000.  Memory: bitmap for sums up to roughly twice
 * the prefix sum, plus a fixed array of terms. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_CAP 20000            /* enough for all required n */

/* Storage for terms and prefix sums.  Index 1..MAX_CAP. */
static seq_t seq[MAX_CAP + 1];
static seq_t pref[MAX_CAP + 1];
static idx_t curr_len = 0;       /* number of terms constructed so far */
static seq_t sum_so_far = 0;     /* S(curr_len) */

/* Bitmap of forbidden sums (S(i)+S(j) for previous prefix sums). */
static uint64_t *bitmap = NULL;
static idx_t bitmap_words = 0;   /* number of 64‑bit words allocated */

/* Ensure the bitmap can hold bit 'idx'. */
static void ensure_bit_capacity_uint64(uint64_t idx) {
    idx_t req_word = idx >> 6;                 /* word index */
    if (req_word >= bitmap_words) {
        idx_t new_words = bitmap_words ? bitmap_words : 1;
        while (new_words <= req_word) new_words <<= 1;
        uint64_t *new_bits = (uint64_t *)realloc(bitmap, new_words * sizeof(uint64_t));
        if (!new_bits) abort();
        for (idx_t w = bitmap_words; w < new_words; ++w) new_bits[w] = 0;
        bitmap = new_bits;
        bitmap_words = new_words;
    }
}

/* Set bit 'idx'. */
static inline void set_bit_uint64(uint64_t idx) {
    ensure_bit_capacity_uint64(idx);
    bitmap[idx >> 6] |= (uint64_t)1 << (idx & 63);
}

/* Test whether bit 'idx' is set. */
static inline bool test_bit_uint64(uint64_t idx) {
    idx_t w = idx >> 6;
    if (w >= bitmap_words) return false;
    return (bitmap[w] >> (idx & 63)) & 1ULL;
}

/* Build sequence up to index n (inclusive). */
static void build_upto(idx_t n) {
    if (n > MAX_CAP) abort();  /* capacity exceeded */
    while (curr_len < n) {
        if (curr_len == 0) {        /* a(1) = 1 */
            seq[1] = 1;
            sum_so_far = 1;
            pref[1] = 1;
            set_bit_uint64(2);      /* 1 + 1 */
            ++curr_len;
        } else {
            seq_t cand = seq[curr_len] + 1;
            /* Find smallest allowed candidate > previous term. */
            while (true) {
                uint64_t key = (uint64_t)cand + (uint64_t)sum_so_far;
                if (!test_bit_uint64(key)) break;
                ++cand;
            }
            ++curr_len;
            seq[curr_len] = cand;
            sum_so_far += cand;
            pref[curr_len] = sum_so_far;

            /* Update bitmap with sums involving the new total. */
            seq_t s_new = sum_so_far;
            for (idx_t i = 1; i < curr_len; ++i) {
                set_bit_uint64((uint64_t)s_new + (uint64_t)pref[i]);
            }
            set_bit_uint64((uint64_t)s_new + (uint64_t)s_new);
        }
    }
}

/* Public function required by the evaluation harness. */
seq_t a(idx_t n) {
    if (n == 0) return 0;     /* undefined for 0, guard */
    build_upto(n);
    return seq[n];
}
