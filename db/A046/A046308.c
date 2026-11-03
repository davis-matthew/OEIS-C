/*
 * A046308  Numbers that are divisible by exactly 7 primes counting
 *          multiplicity.
 * https://oeis.org/A046308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1880, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9888 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046308: Numbers that are divisible by exactly 7 primes counting multiplicity. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/mem.h"

static void compute_list(idx_t limit, seq_t **out, size_t *out_len)
{
    /* Allocate SPF (smallest prime factor) array */
    uint32_t *spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) return;
    /* Allocate bigomega array */
    uint8_t *bigomega = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!bigomega) { free(spf); return; }

    /* Initialise SPF */
    for (idx_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;
    spf[0] = 0;
    spf[1] = 1;

    /* Sieve to fill SPF */
    idx_t sqrtL = (idx_t) sqrt((double)limit);
    for (idx_t i = 2; i <= sqrtL; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }

    /* Compute bigomega (Ω(n)) */
    bigomega[0] = 0;
    bigomega[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        bigomega[i] = bigomega[i / spf[i]] + 1;
    }

    /* Estimate required capacity for the result array */
    size_t cap = limit / 30 + 10;             /* rough density 3% */
    seq_t *vec = mem_new(seq_t, cap);
    if (!vec) { free(spf); free(bigomega); return; }
    size_t len = 0;

    /* Collect numbers with bigomega == 7 */
    for (idx_t i = 2; i <= limit; ++i) {
        if (bigomega[i] == 7) {
            if (len == cap) {
                cap *= 2;
                seq_t *tmp = mem_resize(seq_t, vec, cap);
                if (!tmp) { mem_free(vec); free(spf); free(bigomega); return; }
                vec = tmp;
            }
            vec[len++] = (seq_t)i;
        }
    }

    /* Clean up temporary arrays */
    free(spf);
    free(bigomega);

    *out = vec;
    *out_len = len;
}

static seq_t *seq = NULL;          /* array of 7‑almost primes */
static size_t seq_len = 0;         /* number of elements in seq */
static idx_t cur_limit = 0;        /* current sieve limit */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */

    /* If never initialized, start with a moderate limit. */
    if (!seq) {
        cur_limit = 1 << 20;       /* start with 1,048,576 */
        compute_list(cur_limit, &seq, &seq_len);
    }

    /* Extend the list until it contains at least n terms. */
    while (n > seq_len) {
        cur_limit <<= 1;           /* double the limit each time */
        seq_t *new_seq = NULL;
        size_t new_len = 0;
        compute_list(cur_limit, &new_seq, &new_len);
        mem_free(seq);
        seq = new_seq;
        seq_len = new_len;
    }

    return seq[n - 1];              /* 1‑based indexing */
}
