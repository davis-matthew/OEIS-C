/*
 * A105501  Numbers n such that 1 is the leading digit of the n-th Fibonacci
 *          number in decimal representation.
 * https://oeis.org/A105501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105501: a(n) is the n-th Fibonacci index whose decimal
   leading digit is 1. The computation uses exact Fibonacci numbers
   for small indices (k ≤ 100) and the Benford (fractional‑part)
   test for larger ones. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* Mathematical constants (long double) */
static const long double LOG_PHI  = 0.208987640249978733860L;  /* log10((1+sqrt5)/2) */
static const long double LOG_SQRT5 = 0.349485002168009400084L;  /* log10(sqrt(5))      */
static const long double LOG_2    = 0.301029995663981195213L;  /* log10(2)             */

/* Maximum index for which we compute Fibonacci exactly. */
#define MAX_EXACT 100

/* Internal storage of the sequence indices.  Stored as idx_t to avoid
   overflow on very large indices; cast to seq_t when returning. */
static idx_t *terms = NULL;   /* array of indices (F_k) */
static idx_t term_cnt = 0;    /* number of terms stored   */
static idx_t term_cap = 0;    /* allocated capacity       */

/* Last Fibonacci index processed.  Begins at MAX_EXACT after precomputation. */
static idx_t last_k = 0;

/* Fractional part of log10(F_k) for the current last_k:
   frac_k = frac(k*LOG_PHI - LOG_SQRT5). */
static long double frac_part = 0.0L;

/* Forward declarations */
static void init(void);
static void grow_terms(void);

/* Init on first call: precompute small Fibonacci numbers and
   initialise fractional part. */
static void init(void)
{
    if (terms != NULL) return;            /* already initialised */

    /* Allocate initial capacity for the sequence array. */
    term_cap = 32;
    terms = (idx_t *)malloc(term_cap * sizeof(idx_t));

    /* Compute Fibonacci numbers exactly up to MAX_EXACT using
       unsigned __int128 (fits F_100).  Detect leading digit 1. */
    unsigned __int128 f_prev = 1;      /* F_1 */
    unsigned __int128 f_cur  = 1;      /* F_2 */

    /* k = 1 */
    if (f_prev == 1) {                 /* leading digit is 1 */
        if (term_cnt >= term_cap) grow_terms();
        terms[term_cnt++] = 1;
    }

    /* k = 2 */
    if (f_cur == 1) {
        if (term_cnt >= term_cap) grow_terms();
        terms[term_cnt++] = 2;
    }

    /* k from 3 to MAX_EXACT */
    for (idx_t k = 3; k <= MAX_EXACT; ++k) {
        unsigned __int128 f_next = f_prev + f_cur;
        /* Determine leading decimal digit of f_next */
        unsigned __int128 tmp = f_next;
        while (tmp >= 10) tmp /= 10;
        if (tmp == 1) {
            if (term_cnt >= term_cap) grow_terms();
            terms[term_cnt++] = k;
        }
        f_prev = f_cur;
        f_cur = f_next;
    }

    /* Set last processed index to MAX_EXACT. */
    last_k = MAX_EXACT;

    /* Initialise fractional part for k = MAX_EXACT. */
    long double val = last_k * LOG_PHI - LOG_SQRT5;
    frac_part = val - floorl(val);
    if (frac_part < 0.0L) frac_part += 1.0L;   /* safety for negative */
}

/* Double the capacity of the sequence array. */
static void grow_terms(void)
{
    term_cap <<= 1;
    terms = (idx_t *)realloc(terms, term_cap * sizeof(idx_t));
}

/* Public function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* convention for indices < 1 */
    init();

    /* Generate terms until we have at least n of them. */
    while (term_cnt < n) {
        /* Advance to next Fibonacci index. */
        ++last_k;

        /* Determine leading digit by Benford's formula. */
        frac_part += LOG_PHI;
        if (frac_part >= 1.0L) frac_part -= 1.0L;

        /* Leading digit is 1 if fractional part < log10(2). */
        if (frac_part < LOG_2) {
            if (term_cnt >= term_cap) grow_terms();
            terms[term_cnt++] = last_k;
        }
    }

    /* Return the requested term cast to seq_t. */
    return (seq_t)terms[n - 1];
}
