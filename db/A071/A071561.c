/*
 * A071561  Numbers with no middle divisors (cf. A071090).
 * https://oeis.org/A071561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A071561: Numbers with no middle divisors
 *  a(n) = the n‑th integer k > 2 for which no divisor d of k satisfies
 *         floor(sqrt(k/2)) < d <= floor(sqrt(2k)).
 *  This implementation tests each candidate k in increasing order and
 *  memoises the found terms for fast subsequent look‑ups.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt_seq.h"   /* floor_sqrt_seq(seq_t v) */

static seq_t *terms = NULL;          /* stored terms, 0‑based index   */
static size_t terms_cnt = 0;         /* number of stored terms        */
static size_t terms_cap = 0;         /* allocated capacity            */
static idx_t last_checked = 2;       /* last integer examined (k>=3)  */

/*  Ensure the array has room for `need` more elements.  */
static void ensure_terms_cap(size_t need)
{
    if (terms_cap >= terms_cnt + need)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
    while (new_cap < terms_cnt + need)
        new_cap *= 2;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/*  Return the n-th term of A071561 (1‑based index).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* not used, for safety */

    /* generate terms until we have at least n of them */
    while (terms_cnt < n) {
        ++last_checked;
        seq_t k = last_checked;

        /* lower bound for a potential middle divisor:
         * d > floor(sqrt(k/2))   →  d >= floor_sqrt(k/2)+1         */
        seq_t half = k >> 1;                       /* k / 2   (integer) */
        seq_t sqrt_half = floor_sqrt_seq(half);
        idx_t low  = (idx_t)sqrt_half + 1;

        /* upper bound for a potential middle divisor:
         * d <= floor(sqrt(2k))  →  d <= floor_sqrt(2k)          */
        seq_t two_k = k + k;                       /* 2 * k   (avoid overflow with shift) */
        seq_t sqrt_two_k = floor_sqrt_seq(two_k);
        idx_t high = (idx_t)sqrt_two_k;

        bool ok = true;
        for (idx_t d = low; d <= high; ++d) {
            if (k % d == 0) {               /* found a middle divisor */
                ok = false;
                break;
            }
        }

        if (ok) {                           /* k has no middle divisor */
            ensure_terms_cap(1);
            terms[terms_cnt++] = k;
        }
    }

    return terms[n - 1];                    /* offset 1: a(1) is the first stored term */
}
