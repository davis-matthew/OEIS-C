/*
 * A085651  Index of the first of two successive 2's in A005361.
 * https://oeis.org/A085651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 2334, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A085651 – the index of the first of two successive 
    numbers having exactly one repeated prime factor (bigomega(n)–omega(n)=1).
    The algorithm pre‑computes all such indices up to a dynamically
    enlarged bound using a sieve for the smallest prime factor and
    factorises each integer to test the condition.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t  global_limit = 0;        /* current sieving limit            */
static idx_t *global_spf   = NULL;     /* smallest prime factor array      */
static unsigned char *global_good = NULL; /* good[i] = (sum(e-1)==1)      */
static seq_t  *global_terms = NULL;   /* list of indices where good[i] && good[i+1]  */
static size_t global_terms_cnt = 0;   /* number of terms stored          */
static size_t global_terms_cap = 0;   /* capacity of global_terms array   */

/* Helper: build tables up to a new limit (new_limit should be > old_limit)   */
static void build_tables(idx_t new_limit) {
    /* free previous tables */
    if (global_spf)     free(global_spf);
    if (global_good)    free(global_good);
    if (global_terms)   free(global_terms);

    global_limit = new_limit;
    global_spf = (idx_t*)malloc((global_limit + 2) * sizeof(idx_t));
    global_good = (unsigned char*)malloc((global_limit + 2) * sizeof(unsigned char));

    /* linear sieve for smallest prime factor (spf) */
    for (idx_t i = 2; i <= global_limit; ++i) global_spf[i] = i;
    for (idx_t i = 2; i * i <= global_limit; ++i) {
        if (global_spf[i] == i) {                 /* i is prime */
            for (idx_t j = i * i; j <= global_limit; j += i)
                if (global_spf[j] == j) global_spf[j] = i;
        }
    }

    /* compute "good" flag: sum(e_i – 1) == 1    */
    global_good[0] = global_good[1] = 0;
    for (idx_t n = 2; n <= global_limit; ++n) {
        idx_t x = n;
        int sum = 0;
        while (x > 1) {
            idx_t p = global_spf[x];
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            sum += e - 1;
            if (sum > 1) break;             /* early exit */
        }
        global_good[n] = (sum == 1) ? 1U : 0U;
    }

    /* build the terms vector: indices where good[i] && good[i+1] */
    global_terms_cap = 0;
    global_terms_cnt = 0;
    global_terms = NULL;
    for (idx_t i = 1; i < global_limit; ++i) {
        if (global_good[i] && global_good[i + 1]) {
            if (global_terms_cnt >= global_terms_cap) {
                size_t newcap = global_terms_cap ? global_terms_cap * 2 : 1024;
                seq_t *tmp = (seq_t*)realloc(global_terms, newcap * sizeof(seq_t));
                global_terms = tmp;
                global_terms_cap = newcap;
            }
            global_terms[global_terms_cnt++] = (seq_t)i;
        }
    }
}

/* Main function expected by the test harness */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* not defined for offset 0 */

    /* initialise once with a reasonable upper bound */
    if (global_limit == 0) {
        idx_t init_limit = 600000;          /* covers > 10 000 terms safely */
        build_tables(init_limit);
    }

    /* extend tables if request is beyond current terms */
    while (global_terms_cnt < n) {
        /* double the limit until enough terms are found */
        idx_t new_limit = global_limit * 2;
        build_tables(new_limit);
    }

    /* indexes are 1‑based in the OEIS file */
    return global_terms[n - 1];
}
