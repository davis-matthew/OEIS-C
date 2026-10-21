/*
 * A097643  Numbers n such that n=(d_1!!+d_2!!+...+d_k!!)-(d_1+d_2+...+d_k)
 *          where d_1 d_2 ... d_k is the decimal expansion of n.
 * https://oeis.org/A097643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, base, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A097643
    Definition:  n is in the sequence iff
        n = ( Σ d_i!! ) – ( Σ d_i ),
    where d_1 d_2 … d_k is the decimal expansion of n and x!! denotes the
    double‑factorial of the single digit x.
    Example: 944 = (9!!+4!!+4!!)-(9+4+4).
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static const unsigned short double_fact[10] = {
    1,   /* 0!! = 1   */
    1,   /* 1!! = 1   */
    2,   /* 2!! = 2   */
    3,   /* 3!! = 3   */
    8,   /* 4!! = 4*2 = 8   */
    15,  /* 5!! = 5*3*1 = 15*/
    48,  /* 6!! = 6*4*2 = 48*/
    105, /* 7!! = 7*5*3*1 = 105*/
    384, /* 8!! = 8*6*4*2 = 384*/
    945  /* 9!! = 9*7*5*3*1 = 945*/
};

seq_t a(idx_t n)
{
    /*  Static storage for memoisation  */
    static seq_t *terms = NULL;         /* array of found terms           */
    static idx_t terms_cap = 0;         /* current capacity of the array  */
    static idx_t terms_len = 0;         /* number of terms stored         */
    static seq_t last_checked = 0;      /* last integer inspected         */
    static bool init_done = false;      /* one‑time initialisation flag   */

    /*  Initialise the cache on first call  */
    if (!init_done) {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms_len = 0;
        last_checked = 0;
        init_done = true;
    }

    /*  Generate terms until we have enough  */
    while ((idx_t)terms_len < n) {
        /*  Next candidate integer  */
        last_checked = last_checked + 1;
        seq_t cand = last_checked;

        /*  Convert candidate to unsigned long long for digit extraction.
            This cast is safe because the sequence terms that we need to
            find (up to index 10000) are far below 2^64.  */
        unsigned long long c = (unsigned long long)cand;

        /*  Sum of digits and sum of double‑factorials of digits  */
        unsigned long long sum_digits = 0;
        unsigned long long sum_dfact = 0;
        unsigned long long tmp = c;
        do {
            unsigned int digit = (unsigned int)(tmp % 10);
            sum_digits += digit;
            sum_dfact += double_fact[digit];
            tmp /= 10;
        } while (tmp > 0);

        seq_t val = (seq_t)(sum_dfact - sum_digits);
        if (val == cand) {
            /*  Store term; grow array if necessary  */
            if (terms_len >= terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len] = cand;
            terms_len++;
        }
    }

    return terms[n - 1];
}
