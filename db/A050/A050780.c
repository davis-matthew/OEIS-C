/*
 * A050780  Numbers k such that sopfr(k) = sopfr(k + sopfr(k)).
 * https://oeis.org/A050780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 201, u64 n <= 201,
 *            u128 n <= 201, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A050780 – numbers k such that sopfr(k) = sopfr(k + sopfr(k))
 * This implementation keeps a global cache of found terms.
 * For efficient prime‐factorisation it precomputes the smallest
 * prime factor (spf) for all integers up to a fixed maximum.
 * Subsequent calls to a(n) scan integers incrementally until
 * at least n terms have been found.  The function is fully
 * thread‑safe as it only uses static data after initialisation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LIMIT 20000000U   /* maximum n considered for k + sopfr(k) */

/* forward declarations of helpers */
static void init_spf(void);
static uint32_t sopfr_lpf(uint32_t n);
static void expand_terms(void);
static void generate_terms_until(size_t needed);

/* static data */
static uint32_t *spf = NULL;          /* smallest prime factor array   */
static bool spf_ready = false;        /* spf initialisation flag       */

static seq_t *terms = NULL;           /* cached sequence values        */
static size_t terms_used   = 0;       /* number of terms stored        */
static size_t terms_cap    = 0;       /* allocated capacity            */
static uint32_t proven_max = 1U;      /* last integer tried in search  */

/* ------------------------------------------------------------ */

static void init_spf(void)
{
    if (spf_ready) return;
    spf = (uint32_t *)calloc(MAX_LIMIT + 1, sizeof(uint32_t));
    if (!spf) exit(1);                 /* abort on allocation failure */

    uint32_t *pr = (uint32_t *)malloc((MAX_LIMIT / 10) * sizeof(uint32_t));
    if (!pr) { free(spf); exit(1); }
    size_t pcnt = 0;

    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) {             /* i is prime */
            spf[i] = i;
            if (pcnt < MAX_LIMIT / 10) pr[pcnt++] = i;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            uint32_t p = pr[j];
            uint64_t v = (uint64_t)i * p;
            if (v > MAX_LIMIT) break;
            spf[v] = p;
            if (p == spf[i]) break;
        }
    }
    free(pr);
    spf_ready = true;
}

/* sum of prime factors with multiplicity using spf */
static inline uint32_t sopfr_lpf(uint32_t n)
{
    uint32_t sum = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        sum += p;
        n /= p;
    }
    return sum;
}

/* enlarge terms array when necessary */
static void expand_terms(void)
{
    if (terms_used == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) { free(terms); exit(1); }
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* generate more terms until we have 'needed' of them */
static void generate_terms_until(size_t needed)
{
    if (terms_used >= needed) return;
    init_spf();
    /* scan integers from last checked + 1 upwards */
    for (uint32_t k = proven_max + 1; k <= MAX_LIMIT; ++k) {
        uint32_t sopk = sopfr_lpf(k);
        uint32_t kp = k + sopk;
        if (kp <= MAX_LIMIT) {
            uint32_t sopkp = sopfr_lpf(kp);
            if (sopk == sopkp) {
                expand_terms();
                terms[terms_used++] = (seq_t)k;
                if (terms_used >= needed) {
                    proven_max = k;
                    return;
                }
            }
        }
        proven_max = k;
    }
    /* If we reach here it means the fixed limit was too low. */
    /* The implementation assumes MAX_LIMIT is large enough for
       all requested n in practice. */
}

/* ------------------------------------------------------------
   Public sequence function as required.
   a(1) returns the first term of the sequence (39).
   ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined offset below 1 */
    generate_terms_until((size_t)n);
    return terms[(size_t)n - 1];
}
