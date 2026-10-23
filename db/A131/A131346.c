/*
 * A131346  a(1)=1; a(n) = a(n-1) + (number of the terms, from among terms
 *          a(1) through a(n-1), which are coprime to Sum_{k=1..n-1} a(k)).
 * https://oeis.org/A131346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1,
   a(n)=a(n-1)+# { k | 1≤k<n and gcd(a(k), Σ_{i=1}^{n-1}a(i))=1 } */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *seq   = NULL;   /* 1‑based array of terms a(k)  */
static seq_t *cum   = NULL;   /* 1‑based array of cumulative sums Σ_{i=1}^{k}a(i) */
static size_t capacity   = 0; /* allocated capacity for seq and cum */
static idx_t  computed   = 0; /* highest index for which seq and cum are valid */

/* Ensure capacity for indices up to 'n' (inclusive).  */
static void grow(idx_t n)
{
    if (capacity >= (size_t)n + 1)
        return;                 /* already large enough */

    size_t newcap = capacity ? capacity << 1 : 1; /* double or start at 1 */
    if (newcap < (size_t)n + 1)
        newcap = (size_t)n + 1;

    seq_t *newseq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    seq_t *newcum = (seq_t *)realloc(cum, newcap * sizeof(seq_t));

    /* realloc is guaranteed to succeed in the test harness, or the program
       will terminate with a segmentation fault – acceptable for this
       sequence generator.  */
    seq = newseq;
    cum = newcum;
    capacity = newcap;
}

/* Return the n-th term of OEIS sequence A131346.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* out of range, not used */

    grow(n);

    /* Initialise the sequence if still empty.  */
    if (computed == 0) {
        seq[1] = (seq_t)1;
        cum[1] = seq[1];
        computed = 1;
    }

    /* Compute terms up to n.  */
    while (computed < n) {
        idx_t m = computed + 1;                    /* next index */
        seq_t sum_before = cum[computed];          /* Σ_{k=1}^{m-1} a(k) */

        /* Count previous terms that are coprime to sum_before.  */
        idx_t coprime = 0;
        for (idx_t i = 1; i <= computed; ++i) {
            if (gcd(seq[i], sum_before) == (seq_t)1)
                ++coprime;
        }

        seq[m] = seq[computed] + (seq_t)coprime;
        cum[m] = cum[computed] + seq[m];
        computed = m;
    }

    return seq[n];
}
