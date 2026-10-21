/*
 * A054998  Integers that can be expressed as the sum of consecutive primes in
 *          exactly 3 ways.
 * https://oeis.org/A054998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1643, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
/// A054998 – Integers that can be expressed as the sum of consecutive primes in exactly 3
/// ways.
/// The algorithm enumerates all sums of consecutive primes up to a current limit,
/// counts how many times each sum occurs, and records those sums that occur exactly
/// three times.  The limit is increased only when more terms are requested.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- static state ---------- */
static seq_t *SEQ = NULL;          /* sequence terms (0‑based index for storage) */
static size_t SEQ_LEN = 0;         /* number of stored terms */
static size_t SEQ_CAP = 0;         /* allocated capacity */

static size_t CUR_LIMIT = 0;       /* maximum integer value processed */

/* ---------- memory helpers ---------- */
static void *xmalloc(size_t size)
{
    void *p = malloc(size);
    if (!p) exit(EXIT_FAILURE);
    return p;
}
static void *xrealloc(void *p, size_t size)
{
    void *q = realloc(p, size);
    if (!q) exit(EXIT_FAILURE);
    return q;
}

/* ---------- push_back into dynamic SEQ ---------- */
static void seq_push(seq_t val)
{
    if (SEQ_LEN == SEQ_CAP) {
        size_t new_cap = SEQ_CAP ? SEQ_CAP * 2 : 64;
        SEQ = (seq_t *)xrealloc(SEQ, new_cap * sizeof(seq_t));
        SEQ_CAP = new_cap;
    }
    SEQ[SEQ_LEN++] = val;
}

/* ---------- simple sieve of Eratosthenes, no sqrt() or log() ---------- */
static void sieve_primes(size_t limit, int **primes_out, size_t *cnt_out)
{
    /* use a bit array of uint8_t: 0 = prime, 1 = composite */
    uint8_t *is_comp = (uint8_t *)xmalloc(limit + 1);
    memset(is_comp, 0, limit + 1);

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* collect primes into a dynamic array */
    int *primes = (int *)xmalloc(limit / 10 * sizeof(int));  /* rough initial size */
    size_t cnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (cnt == (size_t)(limit / 10 * sizeof(int))) {
                primes = (int *)xrealloc(primes, (cnt * 3 / 2 + 100) * sizeof(int));
            }
            primes[cnt++] = (int)i;
        }
    }

    free(is_comp);
    *primes_out = primes;
    *cnt_out = cnt;
}

/* ---------- recompute the sequence up to a given numeric limit ---------- */
static void compute_sequence(size_t limit)
{
    /* free any previous data */
    free(SEQ);
    SEQ = NULL;
    SEQ_LEN = SEQ_CAP = 0;
    CUR_LIMIT = limit;

    /* 1. generate primes up to the limit */
    int *primes = NULL;
    size_t prime_cnt = 0;
    sieve_primes(limit, &primes, &prime_cnt);

    /* 2. counts of representations for each sum */
    uint16_t *counts = (uint16_t *)xmalloc((limit + 1) * sizeof(uint16_t));
    memset(counts, 0, (limit + 1) * sizeof(uint16_t));

    /* 3. enumerate all sums of consecutive primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t sum = 0;
        for (size_t j = i; j < prime_cnt; ++j) {
            sum += (uint64_t)primes[j];
            if (sum > limit) break;
            ++counts[(size_t)sum];
        }
    }

    /* 4. collect all sums that occur exactly 3 times */
    for (size_t x = 2; x <= limit; ++x) {
        if (counts[x] == 3) seq_push((seq_t)x);
    }

    free(primes);
    free(counts);
}

/* ---------- provide at least n terms in SEQ ---------- */
static void ensure_n_terms(idx_t n)
{
    if (SEQ_LEN >= (size_t)n) return;

    size_t limit = 5000000;            /* initial numeric limit */
    compute_sequence(limit);

    /* increase limit until enough terms are found */
    while (SEQ_LEN < (size_t)n) {
        limit <<= 1;                  /* double the limit */
        compute_sequence(limit);
    }
}

/* ---------- public interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, guard */
    ensure_n_terms(n);
    return SEQ[n - 1];                     /* adjust for 1‑based indexing */
}
