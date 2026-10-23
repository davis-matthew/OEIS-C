/*
 * A105741  For n>2, a(n) > 0 is such that a(n-1)^2+4*a(n-2)*a(n) is a minimal
 *          square, a(1)=1,a(2)=8.
 * https://oeis.org/A105741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A105741: a(1)=1, a(2)=8.
    For n>2, a(n) is the smallest positive integer such that
    a(n-1)^2 + 4*a(n-2)*a(n) is a perfect square.  That square is
    the minimal possible one.  This is computed by searching for the
    first m≥1 with

        m*(m+2*a(n-1)) ≡ 0  (mod 4*a(n-2))

    and returning
        a(n) = m*(m+2*a(n-1)) / (4*a(n-2))   */
#include <stdlib.h>

static seq_t *a_seq = NULL;    /* cached terms, 1‑based indexing */
static idx_t  a_used = 0;      /* current highest index stored */
static idx_t  a_cap   = 0;      /* current allocation size */

/* allocate or grow the static array to at least size `n` */
static void ensure_capacity(idx_t n)
{
    if (n <= a_cap) return;
    idx_t new_cap = a_cap * 2 + 2;          /* >1 */
    while (new_cap < n) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(a_seq, (new_cap + 1) * sizeof(seq_t));
    if (!new_arr) abort();                 /* memory exhausted */
    a_seq = new_arr;
    a_cap = new_cap;
}

/* compute a(n) for all missing indices up to `n`.  The function
   keeps a static memoization table so that calls in any order are
   efficient. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined but guard */
    if (a_seq == NULL) {
        a_seq = (seq_t *)calloc(3, sizeof(seq_t));   /* indices 0..2 */
        if (!a_seq) abort();
        a_cap = 2;                      /* we have entries for 0..2 */
        a_seq[1] = (seq_t)1;
        a_seq[2] = (seq_t)8;
        a_used = 2;
    }
    ensure_capacity(n);
    while (a_used < n) {
        idx_t k = a_used + 1;            /* next index to compute */
        if (k == 1) a_seq[1] = (seq_t)1;
        else if (k == 2) a_seq[2] = (seq_t)8;
        else {
            seq_t prev2 = a_seq[k-2];    /* a(k-2) */
            seq_t prev1 = a_seq[k-1];    /* a(k-1) */
            seq_t denom = (seq_t)4 * prev2;
            seq_t m = 1;
            for (;; ++m) {
                seq_t num = m * (m + (seq_t)2 * prev1);
                if (num % denom == 0) {
                    a_seq[k] = num / denom;
                    break;
                }
            }
        }
        ++a_used;
    }
    return a_seq[n];
}
