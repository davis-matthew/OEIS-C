/*
 * A102825  "True already", base 3, start 0: a(n) is the least integer such
 *          that the sequence up to a(n-1) written in base 3 contains
 *          floor(a(n)/3) copies of the digit a(n) % 3, with a(0) = 0.
 * https://oeis.org/A102825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the least integer m such that floor(m/3) equals the
 *            number of occurrences of the digit (m mod 3) in the base‑3
 *            representations of a(0), a(1), ..., a(m‑1). The sequence is
 *            calculated by maintaining counts of the digits 0,1,2 and using
 *            the recurrence a(n) = min( 3*cnt[0], 3*cnt[1] + 1, 3*cn...
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *sequence   = NULL;   /* array of generated terms   */
static idx_t   capacity  = 0;      /* allocated array capacity  */
static idx_t   size      = 0;      /* number of terms stored    */
static seq_t   cnt[3]    = {0,0,0};/* counts of digits 0,1,2    */

/* helper to add the base‑3 digits of x to the count array */
static void update_counts(seq_t x)
{
    if (x == (seq_t)0) {
        cnt[0] = cnt[0] + (seq_t)1;
        return;
    }
    seq_t t = x;
    while (t > (seq_t)0) {
        int d = (int)(t % (seq_t)3);
        cnt[d] = cnt[d] + (seq_t)1;
        t = t / (seq_t)3;
    }
}

/* helper to grow the sequence storage as needed */
static void ensure_capacity(idx_t needed)
{
    if (needed <= capacity) return;
    idx_t new_cap = (capacity == 0) ? 16 : capacity * 2;
    while (new_cap <= needed) new_cap *= 2;
    seq_t *new_seq = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (!new_seq) abort(); /* out of memory */
    for (idx_t i = 0; i < size; ++i) new_seq[i] = sequence[i];
    free(sequence);
    sequence = new_seq;
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise for first call */
    if (size == 0) {
        ensure_capacity(16);
        sequence[0] = (seq_t)0;
        size = 1;
        cnt[0] = (seq_t)1; /* base‑3 representation of 0 is "0" */
        cnt[1] = (seq_t)0;
        cnt[2] = (seq_t)0;
        if (n == 0) return sequence[0];
    }

    /* compute terms up to n if needed */
    while (size <= n) {
        /* candidates for the next term */
        seq_t cand0 = cnt[0] * (seq_t)3;          /* residue 0 */
        seq_t cand1 = cnt[1] * (seq_t)3 + (seq_t)1; /* residue 1 */
        seq_t cand2 = cnt[2] * (seq_t)3 + (seq_t)2; /* residue 2 */

        seq_t next = cand0;
        if (cand1 < next) next = cand1;
        if (cand2 < next) next = cand2;

        /* store the term */
        ensure_capacity(size);
        sequence[size] = next;

        /* update counts with the new term's digits */
        update_counts(next);

        ++size;
    }

    return sequence[n];
}
