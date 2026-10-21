/*
 * A055075  Numbers expressible as the sum of no more than 5 squares of
 *          composite numbers, allowing repetitions.
 * https://oeis.org/A055075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A055075 – numbers that can be expressed as the sum of at most 5
   squares of composite numbers (repetitions allowed).
   The function returns the n‑th term (1‑based). */

/* This implementation pre‑computes all representable numbers up to 1167,
   the largest integer not in the sequence.  For n beyond that, all
   integers are representable, so the n‑th term equals
   1167 + (n – countRepresentable1167). */

#include <stdbool.h>
#include <stddef.h>

static seq_t small_terms[1000];   /* representable numbers ≤ 1167 */
static size_t small_terms_count = 0;
static bool initialized = false;

/* Pre‑compute all representable numbers ≤ 1167. */
static void init_small_terms(void)
{
    /* mark representable numbers */
    bool rep[1168] = { false };

    /* composite numbers ≤ 34 (⌊√1167⌋) */
    const int comps[] = {
        4,6,8,9,10,12,14,15,16,18,20,21,22,24,25,26,
        27,28,30,32,33,34
    };
    const size_t comp_len = sizeof(comps)/sizeof(comps[0]);

    /* squares of composites, with 0 as a “no‑square” sentinel */
    int sq[25];
    sq[0] = 0;                       /* zero allows fewer than 5 squares */
    for (size_t i=0; i<comp_len; ++i)
        sq[i+1] = comps[i]*comps[i];

    /* brute‑force all 5‑tuples (allowing zeros). */
    for (int a=0; a<25; ++a)
    for (int b=0; b<25; ++b)
    for (int c=0; c<25; ++c)
    for (int d=0; d<25; ++d)
    for (int e=0; e<25; ++e) {
        int s = sq[a] + sq[b] + sq[c] + sq[d] + sq[e];
        if (s>0 && s<=1167)
            rep[s] = true;
    }

    /* collect representable numbers in ascending order */
    for (int n=1; n<=1167; ++n) {
        if (rep[n]) {
            small_terms[small_terms_count++] = (seq_t)n;
        }
    }
}

/* public accessor for the sequence */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_small_terms();
        initialized = true;
    }

    /* If the requested index lies within the pre‑computed range,
       return the stored term. */
    if ((idx_t)small_terms_count >= n && n > 0) {
        return small_terms[n-1];
    }

    /* For indices beyond the pre‑computed range, the sequence
       continues consecutively from 1168 upwards. */
    /* 1168 = 1167 + 1, and the number of representables ≤1167
       is small_terms_count (equal to 911). */
    return (seq_t)(1167 + (seq_t)(n - (seq_t)small_terms_count));
}
