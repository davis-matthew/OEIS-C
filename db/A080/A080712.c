/*
 * A080712  a(0) = 4; for n>0, a(n) is taken to be the smallest positive
 *          integer greater than a(n-1) which is consistent with the condition
 *          "n is a member of the sequence if and only if a(n) is a multiple
 *          of 3".
 * https://oeis.org/A080712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 66 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A080712
    a(0) = 4;
    For n > 0, a(n) is the smallest integer > a(n‑1) such that
      (a(n) % 3 == 0)  ⇔  (n is a term of the sequence).
    The sequence is built incrementally; membership of an index is tested
    by binary searching the already computed values.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/*  binary search: does value appear in seq[0..len-1]?  */
static bool exists_in_seq(const seq_t *array, idx_t len, idx_t value)
{
    idx_t low = 0, high = len;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((seq_t)mid > array[mid]) { /* this comparison uses numeric value */
            /* not needed; values are sorted strictly increasing */
            /* but we compare value with array[mid] directly */
        }
        if (array[mid] == (seq_t)value)
            return true;
        else if (array[mid] < (seq_t)value)
            low = mid + 1;
        else
            high = mid;
    }
    return false;
}

/*  dynamic array holding the sequence values  */
static seq_t *seq = NULL;      /* array of sequence values */
static idx_t seq_len = 0;      /* number of values stored */
static idx_t seq_cap = 0;      /* allocated capacity */

/*  ensure that seq has space for at least (need) elements  */
static void ensure_seq_capacity(idx_t need)
{
    if (need < seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 1;
    while (new_cap <= need) new_cap <<= 1;
    seq = mem_resize(seq_t, seq, new_cap);
    seq_cap = new_cap;
}

/*  main function required by the harness  */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (seq_len == 0) {
        ensure_seq_capacity(0);
        seq[0] = (seq_t)4;
        seq_len = 1;
    }

    /* extend sequence up to n (if needed) */
    while (seq_len <= n) {
        idx_t i = seq_len;                      /* index to compute */
        bool in_set = exists_in_seq(seq, seq_len, i);

        seq_t cand = seq[i - 1] + (seq_t)1;      /* start just above previous term */
        while ( ((cand % (seq_t)3) == 0) != in_set )
            cand++;

        ensure_seq_capacity(seq_len);
        seq[seq_len++] = cand;
    }

    return seq[n];
}
