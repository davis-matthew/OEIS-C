/*
 * A225332  A continuous "look-and-repeat" sequence (seed 2).
 * https://oeis.org/A225332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is produced by the following “look‑and‑repeat” rule.
 *            Starting with a single digit 2, each time a maximal run of equal
 *            digits of length L and value D is encountered, the triple
 *            (L,L,D) is appended to the end of the sequence. Repeating this
 *            process indefinitely generates the whole sequence. Th...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;      /* array holding the generated sequence   */
static size_t seq_len = 0;     /* number of elements currently stored   */
static size_t seq_cap = 0;     /* allocated capacity                     */
static idx_t  proc_pos = 0;    /* position of next unprocessed run head */

static void init_seq(void) {
    seq_cap = 64;
    seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    seq[0] = (seq_t)2;         /* seed value                      */
    seq_len = 1;
    proc_pos = 0;
}

static void ensure_cap(size_t need) {
    if (need > seq_cap) {
        size_t new_cap = seq_cap;
        while (new_cap < need) new_cap <<= 1;
        seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

static void compute_upto(idx_t N) {
    if (!seq) init_seq();
    /* generate values until we have at least N terms  */
    while (seq_len < (size_t)N) {
        seq_t cur = seq[proc_pos];
        idx_t j = proc_pos;
        /* find the end of the current run of equal digits */
        while (j < (idx_t)seq_len && seq[j] == cur) ++j;
        idx_t run_len = j - proc_pos;
        /* append the triple (run_len, run_len, cur) */
        ensure_cap(seq_len + 3);
        seq[seq_len++] = (seq_t)run_len;
        seq[seq_len++] = (seq_t)run_len;
        seq[seq_len++] = cur;
        /* advance past this run */
        proc_pos += run_len;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for n<1, return 0 */
    compute_upto(n);
    return seq[n-1];
}
