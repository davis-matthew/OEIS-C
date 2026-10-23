/*
 * A044601  Numbers n such that string 4,1 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A044601
   A number n is selected if the string "41" occurs in its octal
   representation while it does not occur in the octal representation of n+1.
   We generate the sequence lazily, storing the values found so far. */

#include <stdlib.h>

static seq_t *seq = NULL;          /* array of found sequence values   */
static size_t seq_size = 0;       /* number of values stored          */
static size_t seq_cap  = 0;       /* current allocation capacity      */
static seq_t last_checked = 0;    /* last integer examined            */

/* Check whether a number x (in base 8) contains the substring "41". */
static int contains41(seq_t x)
{
    /* Max digits of 128‑bit integer in base 8 is < 50, use 64 as a safe buffer. */
    unsigned char rev[64];
    size_t len = 0;

    seq_t tmp = x;
    while (tmp > 0) {
        rev[len++] = (unsigned char)(tmp % (seq_t)8);
        tmp /= (seq_t)8;
    }
    if (len == 0)        /* x == 0 */
        return 0;
    /* rev[0] is least significant digit, rev[len‑1] most significant. */
    for (size_t j = 1; j < len; ++j)
        if (rev[j] == 4 && rev[j - 1] == 1)
            return 1;
    return 0;
}

/* Expand the sequence until it contains at least n terms. */
static void ensure_terms(idx_t n)
{
    if ((size_t)n <= seq_size)
        return;

    if (!seq) {
        seq_cap = 1024;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    while (seq_size < n) {
        seq_t cand = last_checked + 1;
        last_checked = cand;

        if (contains41(cand) && !contains41(cand + 1)) {
            if (seq_size == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_size++] = cand;
        }
    }
}

/* Required external function: compute the nth term (1‑based). */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return seq[n - 1];
}
