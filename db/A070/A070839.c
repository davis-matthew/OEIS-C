/*
 * A070839  Smallest number k such that k + R(k) is a multiple of 11 different
 *          from all previous cases, where R(k) is digit reversal of k
 *          (A004086).
 * https://oeis.org/A070839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The OEIS sequence A070839:  a(n) = the smallest k such that
 *  k + R(k) is a multiple of 11, and the value k + R(k) has not yet
 *  appeared for any smaller n.    (R(k) = decimal reversal of k).
 */
#include <stdlib.h>
#include <stdbool.h>

/* reverse decimal digits of x, dropping leading zeros */
static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        seq_t d = x % 10;
        rev = rev * 10 + d;
        x = x / 10;
    }
    return rev;
}

/* cached terms and the corresponding sums k+R(k) */
static seq_t *terms    = NULL;   /* terms[n-1] = a(n) */
static seq_t *sums     = NULL;   /* sums[n-1] = k + R(k) for that term */
static size_t terms_len = 0;      /* current number of stored terms */
static size_t terms_cap = 0;      /* allocated capacity for terms/sums */
static idx_t   next_k   = 1;      /* smallest candidate k not yet inspected */

/* ensure capacity for at least one more term */
static void grow_terms(void)
{
    size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
    seq_t *t_new = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    seq_t *s_new = (seq_t *)realloc(sums,  new_cap * sizeof(seq_t));
    if (!t_new || !s_new) {
        /* allocation failed – abort */
        abort();
    }
    terms = t_new;
    sums  = s_new;
    terms_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* defined only for n≥1 */

    /* extend precomputation until we have at least n terms */
    while (terms_len < n) {
        if (terms_len == terms_cap) {
            grow_terms();
        }

        seq_t k = next_k++;
        seq_t r = reverse_digits(k);
        seq_t s = k + r;

        if (s % 11 != 0) {
            /* not a candidate – keep searching */
            continue;
        }

        /* check whether this sum has already appeared */
        bool seen = false;
        for (size_t i = 0; i < terms_len; ++i) {
            if (sums[i] == s) {
                seen = true;
                break;
            }
        }
        if (!seen) {
            /* new term – store it */
            terms[terms_len] = k;
            sums[terms_len]  = s;
            terms_len++;
        }
        /* otherwise skip this k and try the next one */
    }

    /* now terms_len ≥ n – answer is stored */
    return terms[n - 1];
}
