/*
 * A071760  Denominators of record absolute values of partial sums of Sum
 *          (mu(n)/n).
 * https://oeis.org/A071760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the n-th record minimum (in absolute
 *            value) of the partial sums S(k) = sum_{i=1}^{k} mu(i)/i for k >
 *            1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   gcd, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/mu_of.h"
#include "../utils/gcd.h"

/* Static caches for record denominators and current sum */
static seq_t *rec_vals = NULL;    /* denominators of record minima */
static size_t rec_len = 0;        /* number of stored records */
static size_t rec_cap = 0;        /* allocated capacity */

static seq_t cur_num_abs = 1;     /* abs numerator of current sum (S(1)=1) */
static seq_t cur_den = 1;         /* denominator of current sum, reduced */
static bool cur_neg = false;      /* true if the current sum is negative */

static seq_t min_abs_num = 0;     /* abs numerator of current minimum */
static seq_t min_abs_den = 1;     /* denominator of current minimum */
static bool min_set = false;      /* true after first record is set */
static idx_t next_n = 2;          /* next n to add to the sum */

/* Ensure the record array can hold at least *need* elements */
static void grow_rec(size_t need)
{
    size_t new_cap = rec_cap ? rec_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    rec_vals = (seq_t *)realloc(rec_vals, new_cap * sizeof(seq_t));
    rec_cap = new_cap;
}

/* Add mu(n)/n to the current sum, storing the reduced result. */
static inline void add_next(idx_t n)
{
    int mu = mu_of(n);                 /* μ(n) ∈ {−1,0,1} */
    seq_t n_seq = (seq_t)n;

    /* Common denominator: cur_den * n */
    seq_t new_den = cur_den * n_seq;

    /* Numerators after scaling to common denominator */
    seq_t t1_abs = cur_num_abs * n_seq; /* contribution of current sum */
    bool t1_sign = cur_neg;            /* sign of current sum */

    seq_t t2_abs = (mu != 0) ? cur_den : 0; /* |mu| * cur_den = cur_den if μ≠0 */
    bool t2_sign = (mu < 0);            /* sign of the μ/n term */

    /* Combine the two signed numerators */
    seq_t new_abs;
    bool new_sign;

    if (t1_abs == 0 && t2_abs == 0) {
        new_abs = 0;
        new_sign = false;
    } else if (t1_sign == t2_sign) {
        new_sign = t1_sign;
        new_abs = t1_abs + t2_abs;
    } else {
        if (t1_abs > t2_abs) {
            new_sign = t1_sign;
            new_abs = t1_abs - t2_abs;
        } else if (t2_abs > t1_abs) {
            new_sign = t2_sign;
            new_abs = t2_abs - t1_abs;
        } else {
            new_abs = 0;
            new_sign = false;
        }
    }

    /* Reduce the fraction */
    if (new_abs != 0) {
        seq_t g = gcd(new_abs, new_den);
        new_abs /= g;
        new_den /= g;
    } else {
        new_den = 1;            /* 0 represented as 0/1 */
    }

    /* Store the result for the next iteration */
    cur_num_abs = new_abs;
    cur_den = new_den;
    cur_neg = new_sign;
}

/* ------------------------------------------------------------ */
/*  Public entry point                                         */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but not used */

    /* Initialise static state on first call */
    if (rec_len == 0) {
        rec_vals = NULL;
        rec_cap = 0;
        rec_len = 0;
        cur_num_abs = 1;  /* sum at n = 1: μ(1)/1 = 1 */
        cur_den = 1;
        cur_neg = false;
        min_set = false;
        next_n = 2;
    }

    /* Compute until we have at least n record denominators */
    while (rec_len < n) {
        add_next(next_n);

        /* Update record if current absolute value is a new minimum */
        if (!min_set ||
            cur_num_abs * min_abs_den < min_abs_num * cur_den) {

            if (rec_len == rec_cap)
                grow_rec(rec_len + 1);
            rec_vals[rec_len++] = cur_den;

            min_abs_num = cur_num_abs;
            min_abs_den = cur_den;
            min_set = true;
        }

        ++next_n;
    }

    return rec_vals[n - 1];
}
