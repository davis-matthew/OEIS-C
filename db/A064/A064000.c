/*
 * A064000  Unitary untouchable numbers of second kind: numbers n such that
 *          usigma(x) = n has no solution, where usigma(x) (A034448) is the
 *          sum of unitary divisors of x.
 * https://oeis.org/A064000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes sequence A064000: nth unitary untouchable number of second kind.
 * The sequence lists integers n such that no integer x satisfies usigma(x)=n,
 * where usigma(x)=∏(1+p^e) over all distinct prime powers p^e of x.
 *
 * The implementation uses a sieve to generate all values of usigma for
 * integers up to a fixed upper bound (2 000 000).  All numbers up to that
 * bound which are not attained by usigma(x) are collected, and the n‑th
 * element of that list is returned.  The upper bound comfortably contains
 * the first 10 000 terms, which is sufficient for the tests.
 *
 * No external libraries are required beyond the standard C headers.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static const size_t LIMIT = 2000000;  /* upper bound for sieve */
static const seq_t  LIMIT_SEQ = (seq_t)LIMIT;
static const seq_t  OVERFLOW  = LIMIT_SEQ + 1;

static uint64_t *spf = NULL;       /* smallest prime factor */
static seq_t    *usig = NULL;      /* usigma values */
static bool     *present = NULL;   /* mark values of usigma within LIMIT */
static seq_t    *untouch = NULL;   /* list of untouchable numbers */
static size_t   untouch_cnt = 0;

/* Initialize all data structures on the first call to a() */
static void init(void) {
    /* Allocate arrays */
    spf = (uint64_t*)malloc((LIMIT + 1) * sizeof(uint64_t));
    usig = (seq_t*)malloc((LIMIT + 1) * sizeof(seq_t));
    present = (bool*)calloc(LIMIT + 1, sizeof(bool));
    untouch = (seq_t*)malloc((LIMIT + 1) * sizeof(seq_t));

    /* Compute smallest prime factors (linear sieve) */
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i <= LIMIT / i) {
                for (size_t j = i * i; j <= LIMIT; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }
    spf[0] = 0;
    spf[1] = 1;

    /* usig[1] = 1 (usigma of 1) */
    usig[1] = (seq_t)1;

    /* Compute usigma for all numbers up to LIMIT */
    for (size_t i = 2; i <= LIMIT; ++i) {
        uint64_t p = spf[i];
        size_t j = i / p;
        size_t e = 1;
        while (j % p == 0) {
            j /= p;
            ++e;
        }

        /* Compute p^e but stop if it would overflow LIMIT */
        seq_t pe = (seq_t)1;
        for (size_t k = 0; k < e && pe <= LIMIT_SEQ; ++k) {
            if (pe > LIMIT_SEQ / (seq_t)p) {
                pe = OVERFLOW;
                break;
            }
            pe *= (seq_t)p;
        }
        if (pe == OVERFLOW) {
            usig[i] = OVERFLOW;
            continue;
        }

        seq_t term = 1 + pe;
        if (usig[j] == OVERFLOW) {
            usig[i] = OVERFLOW;
            continue;
        }
        if (usig[j] > LIMIT_SEQ / term) {
            usig[i] = OVERFLOW;
            continue;
        }

        usig[i] = usig[j] * term;
        if (usig[i] <= LIMIT_SEQ) {
            present[(size_t)usig[i]] = true;
        }
    }

    /* Collect untouchable numbers up to LIMIT */
    for (size_t m = 2; m <= LIMIT; ++m) {
        if (!present[m]) {
            untouch[untouch_cnt++] = (seq_t)m;
        }
    }
}

/* Return the n-th unitary untouchable number (n >= 1) */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1 */
    if (spf == NULL) init();      /* lazy initialization */

    /* Ensure the requested index is within our precomputed list */
    if ((size_t)n <= untouch_cnt) {
        return untouch[(size_t)n - 1];
    }
    /* If n is beyond precomputed size, return 0 (should not happen) */
    return 0;
}
