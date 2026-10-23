/*
 * A123923  a(1)=1. a(n) = (highest term among first n-1 terms of sequence
 *          which divides n)-th integer from among those integers not
 *          occurring among the first n-1 terms of the sequence.
 * https://oeis.org/A123923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (largest previous term dividing n)-th unused positive
 *            integer Recurrence (pseudocode): a(1) = 1 for n > 1: t = max{i |
 *            1 ≤ i < n and a(i) divides n} a(n) = the t‑th positive integer
 *            not in {a(1)…a(n‑1)}
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static state shared by all calls */
static seq_t *seq      = NULL;
static size_t seq_cap  = 0;
static bool *used      = NULL;
static size_t used_cap = 0;
static idx_t computed_until = 0;

/* enlarge sequence storage */
static void ensure_seq_cap(size_t need) {
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* enlarge used array; index 0 is unused */
static void ensure_used_cap(size_t need) {
    if (need <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap < need + 1) new_cap *= 2;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) abort();
    /* zero initialise the new part */
    for (size_t i = used_cap + 1; i <= new_cap; ++i) tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}

/* return the t‑th unused positive integer (t ≥ 1) */
static seq_t get_tth_unused(idx_t t) {
    idx_t missing = 0;
    seq_t val = 0;
    for (idx_t cand = 1; ; ++cand) {
        if ((size_t)cand > used_cap) ensure_used_cap((size_t)cand);
        if (!used[(size_t)cand]) {
            ++missing;
            if (missing == t) {
                val = cand;
                break;
            }
        }
    }
    return val;
}

/* initialise first term if needed */
static void init_first(void) {
    if (computed_until > 0) return;
    ensure_seq_cap(1);
    seq[0] = (seq_t)1;
    ensure_used_cap(1);
    used[1] = true;
    computed_until = 1;
}

/* compute the sequence up to n (inclusive) */
static void compute_up_to(idx_t n) {
    init_first();
    for (idx_t cur = computed_until + 1; cur <= n; ++cur) {
        /* find largest previous term dividing cur */
        idx_t mx_div = 0;
        size_t len = (size_t)cur - 1;
        for (size_t i = 0; i < len; ++i) {
            idx_t term = (idx_t)seq[i];
            if (term && term <= cur && cur % term == 0) {
                if (term > mx_div) mx_div = term;
            }
        }
        /* mx_div is at least 1 because a(1)=1 divides every n */
        seq_t next_val = get_tth_unused(mx_div);
        ensure_seq_cap((size_t)cur);
        seq[cur - 1] = next_val;
        ensure_used_cap((size_t)next_val);
        used[(size_t)next_val] = true;
    }
    computed_until = n;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined, but safe guard */
    if (n > computed_until) compute_up_to(n);
    return seq[n - 1];
}
