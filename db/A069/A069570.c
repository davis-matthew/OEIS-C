/*
 * A069570  Numbers n in which the k-th digit (counted from the right) is
 *          nonzero and either a divisor or a multiple of k, for all 1 <= k <=
 *          number of digits of n.
 * https://oeis.org/A069570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose k-th digit from the right
 *            (k ≥ 1) is non‑zero and either divides k or is a multiple of k.
 *            Numbers are enumerated in ascending order
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

#define MAX_K 20                     /* maximum digit position to pre‑compute */
static int allowed[MAX_K + 1][10];
static int allow_cnt[MAX_K + 1];

/* Dynamic array that holds the sequence */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static int curr_len = 0;             /* length of numbers already generated */

/* initialise the allowed digits tables once */
static void init_allowed(void) {
    for (int k = 1; k <= MAX_K; ++k) {
        int cnt = 0;
        for (int d = 1; d <= 9; ++d) {
            if ((k % d == 0) || (d % k == 0)) {
                allowed[k][cnt++] = d;
            }
        }
        allow_cnt[k] = cnt;
    }
}

/* ensure the sequence buffer can hold at least `needed` entries */
static void ensure_cap(size_t needed) {
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* recursive generator: pos from left (1 … d), cur is current value */
static void gen_rec(int pos, int d, seq_t cur) {
    int k = d - pos + 1;              /* position measured from the right */
    int *digits = allowed[k];
    int cnt = allow_cnt[k];
    for (int i = 0; i < cnt; ++i) {
        seq_t nxt = cur * (seq_t)10 + (seq_t)digits[i];
        if (pos == d) {
            ensure_cap(seq_len + 1);
            seq[seq_len++] = nxt;
        } else {
            gen_rec(pos + 1, d, nxt);
        }
    }
}

/* generate all numbers of exactly `d` digits */
static void generate_len(int d) {
    gen_rec(1, d, (seq_t)0);
}

/* public function required by the grading system */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset 1; n < 1 is undefined */

    if (seq == NULL) {                 /* first call: initialise everything */
        init_allowed();
        seq_cap = 128;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
    }

    /* generate further lengths until we have at least n terms */
    while (seq_len < (size_t)n && curr_len < MAX_K) {
        ++curr_len;
        generate_len(curr_len);
    }

    /* if we have enough terms, return the n‑th (1‑based) */
    if (n <= seq_len) return seq[n - 1];

    /* As a safety net (very rarely reached for typical test sizes) */
    while (seq_len < (size_t)n && curr_len < MAX_K) {
        ++curr_len;
        generate_len(curr_len);
    }
    return seq[n - 1];
}
