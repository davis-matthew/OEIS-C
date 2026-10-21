/*
 * A080639  a(1) = 1; for n>1, a(n) is taken to be the smallest integer
 *          greater than a(n-1) which is consistent with the condition "for
 *          n>1, n is a member of the sequence if and only if a(n) is even".
 * https://oeis.org/A080639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1 ; * for n > 1, a(n) is the smallest integer larger than
 *            a(n‑1) that satisfies * a(n) is even ⇔ n has already appeared as
 *            a value in the sequence. * This implements the definition of
 *            OEIS A080639
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   ensure_member_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_member_capacity.h"

/* Ensure seq array can store indices up to 'needed' (inclusive). */
static void ensure_seq_capacity(idx_t needed, seq_t **seq, size_t *cap)
{
    if ((size_t)needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 16 : *cap;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*seq, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();
    *seq = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for n < 1 */

    /* Static memoisation tables */
    static seq_t *seq_arr   = NULL;
    static size_t seq_cap   = 0;
    static idx_t  seq_last  = 0;

    static bool *member_arr  = NULL;
    static size_t member_cap = 0;

    /* Initialise on first call */
    if (seq_last == 0) {
        ensure_seq_capacity(1, &seq_arr, &seq_cap);
        seq_arr[1] = (seq_t)1;
        seq_last  = 1;

        ensure_member_capacity(1, &member_arr, &member_cap);
        member_arr[1] = true;
    }

    /* Extend tables if required */
    ensure_seq_capacity(n, &seq_arr, &seq_cap);
    ensure_member_capacity(n, &member_arr, &member_cap);

    /* Compute missing terms up to n */
    for (idx_t i = seq_last + 1; i <= n; ++i) {
        seq_t prev = seq_arr[i - 1];
        bool member = false;
        if ((size_t)i <= member_cap) member = member_arr[i];

        seq_t val;
        if (member) {
            /* n already in the image → a(n) must be even */
            val = prev + 1;
            if ((val & 1) != 0) val++;        /* make even */
        } else {
            /* n not yet in the image */
            val = prev + 1;

            if (val == (seq_t)i) {            /* candidate equals n */
                if ((val & 1) != 0) {        /* n is odd → invalid */
                    val += 2;                /* skip to next odd */
                }
                /* else n even → keep val (it is already even) */
            } else {
                /* any other candidate must be odd */
                if ((val & 1) == 0) val++;    /* make odd */
            }
        }

        seq_arr[i] = val;

        /* Record that this value now appears in the sequence */
        size_t val_idx = (size_t)val;
        ensure_member_capacity(val_idx, &member_arr, &member_cap);
        member_arr[val_idx] = true;
    }

    seq_last = n;
    return seq_arr[n];
}
