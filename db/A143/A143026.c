/*
 * A143026  Positive integers k such that the fourth power of the number of
 *          positive divisors of k equals k.
 * https://oeis.org/A143026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143026: a(n) is the nth positive integer k such that
   k = d(k)^4, where d(k) is the number of positive divisors of k.  */
/* The sequence is finite; only the first four terms are known:
   1, 625, 6561, 4100625.  The routine searches for all solutions
   up to a fixed bound and returns the n‑th found value (or 0 if n
   exceeds the number of solutions).  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_seq.h"   /* pow_seq(seq_t base, int exp) */

#define MAX_T 1000000ULL  /* search bound for t in t^4 */

/* Static storage for computed values */
static seq_t *terms = NULL;     /* array of k = t^4 */
static size_t terms_cnt = 0;    /* number of solutions found */
static size_t terms_cap = 0;    /* allocated capacity */
static uint32_t *spf = NULL;    /* smallest prime factor table */
static size_t spf_size = 0;     /* size of spf array */

/* Grow an array of seq_t elements */
static void grow_terms(void)
{
    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (new_arr == NULL) abort();  /* allocation failure */
    terms = new_arr;
    terms_cap = new_cap;
}

/* Build smallest‑prime‑factor table up to MAX_T */
static void build_spf(void)
{
    if (spf != NULL) return;           /* already built */
    spf_size = (size_t)MAX_T + 1;
    spf = (uint32_t *)calloc(spf_size, sizeof(uint32_t));
    if (spf == NULL) abort();

    for (uint64_t i = 2; i <= MAX_T; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= MAX_T) {
                for (uint64_t j = i * i; j <= MAX_T; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
}

/* Append a new term to the terms array */
static void append_term(seq_t val)
{
    if (terms_cnt == terms_cap) grow_terms();
    terms[terms_cnt++] = val;
}

/* Enumerate all solutions up to MAX_T and store them */
static void init_terms(void)
{
    if (terms != NULL) return;   /* already initialized */

    /* Allocate storage for terms */
    terms_cap = 8;
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (terms == NULL) abort();

    build_spf();

    for (uint64_t t = 1; t <= MAX_T; ++t) {
        /* Compute d(t^4) = Π (4*e_i + 1) */
        uint64_t temp = t;
        uint64_t d_t4 = 1;

        while (temp > 1) {
            uint32_t p = spf[temp];
            uint32_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            d_t4 *= (uint64_t)(4 * e + 1);
        }

        if (d_t4 == t) {
            /* k = t^4 */
            seq_t t_big = (seq_t)t;
            seq_t k = pow_seq(t_big, 4); /* t^4 */
            append_term(k);
        }
    }
    /* Release the SPF table if not needed further */
    free(spf);
    spf = NULL;
    spf_size = 0;
}

/* External linkage function required by the evaluation harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1 */
    init_terms();
    if ((idx_t)(n - 1) < (idx_t)terms_cnt)
        return terms[n - 1];
    return 0;                    /* no further terms in the sequence */
}
