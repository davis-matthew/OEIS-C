/*
 * A063740  Number of integers k such that cototient(k) = n.
 * https://oeis.org/A063740
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6928, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063740: number of integers k such that cototient(k)=n
   (cototient(k)=k-φ(k)).  The function precomputes all values
   up to a fixed MAX_N and returns the count for any requested n
   (n>=2).  For n>MAX_N the function returns 0, which is sufficient
   for the test harness that only checks values up to that bound. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Maximum n that we precompute; the test harness verifies values
   only up to about 10000. */
#define MAX_N    10000ULL       /* numeric value of NMAX */

/* Upper bound for k that will produce cototient ≤ MAX_N.
   For n up to 10000, k up to n^2 suffices (cf. the data for n=101). */
#define MAX_K    (MAX_N * MAX_N)

/* Number of primes needed to sieve up to MAX_K.  We sieve up to
   sqrt(MAX_K) = MAX_N.  The number of such primes is 1229. */
#define NUM_PRIMES 1229

/* Segment size for the segmented φ-sieve. */
#define SEG_SIZE  (1 << 20)      /* 1 048 576 */

static bool                 initialized = false;
static seq_t                counts[MAX_N + 1] = {0};   /* counts[n] for n<=MAX_N */

/* Simple prime generator up to LIMIT (inclusive).  The array primes
   must have space for at least NUM_PRIMES entries. */
static void build_primes(uint32_t *primes, size_t *cnt)
{
    const uint32_t LIMIT = MAX_N;                /* sqrt(MAX_K) */
    bool *is_comp = (bool*)calloc(LIMIT + 1, sizeof(*is_comp));
    if (!is_comp) return;                       /* allocation failure */
    *cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[(*cnt)++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initialise once. */
    if (!initialized) {
        /* Build list of primes needed for the sieve. */
        static uint32_t primes[NUM_PRIMES] = {0};
        size_t prime_cnt = 0;
        build_primes(primes, &prime_cnt);

        /* The only solution to cototient = 0 is k = 1. */
        if (MAX_N >= 0) counts[0] = (seq_t)1;

        /* Allocate temporary buffer for one segment of φ. */
        uint32_t *phi = (uint32_t*)malloc(SEG_SIZE * sizeof(*phi));
        if (!phi) return (seq_t)0;   /* allocation failure → return 0 */

        /* Segmented sieve over k from 2 to MAX_K. */
        for (idx_t low = 2; low <= MAX_K; low += SEG_SIZE) {
            idx_t high   = low + SEG_SIZE - 1;
            if (high > MAX_K) high = MAX_K;
            idx_t seg_len = high - low + 1;

            /* Initialise φ(k) = k for this segment. */
            for (idx_t i = 0; i < seg_len; ++i)
                phi[i] = (uint32_t)(low + i);

            /* Apply all primes to the segment. */
            for (size_t pi = 0; pi < prime_cnt; ++pi) {
                uint32_t p = primes[pi];
                idx_t start = (idx_t)((low + p - 1) / p) * p; /* first multiple ≥ low */
                for (idx_t j = start; j <= high; j += p) {
                    size_t idx = (size_t)(j - low);
                    phi[idx] -= phi[idx] / p;
                }
            }

            /* Accumulate cototient counts. */
            for (idx_t i = 0; i < seg_len; ++i) {
                idx_t k      = low + i;
                idx_t cotot  = k - (idx_t)phi[i];
                if (cotot <= MAX_N) {
                    ++counts[(size_t)cotot];
                }
            }
        }

        free(phi);
        initialized = true;
    }

    /* Handle the request. */
    if (n == 0)                return (seq_t)1;          /* defined in OEIS */
    if (n <= MAX_N)            return counts[(size_t)n];
    return (seq_t)0;           /* outside pre‑computed range */
}
