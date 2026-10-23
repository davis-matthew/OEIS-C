/*
 * A162279  Minimal numbers with the same values for sigma_0(n) and sigma(n)
 *          as one or more other numbers.
 * https://oeis.org/A162279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implementation: compute minimal numbers n such that there exists m > n
 * with σ(n) = σ(m) and τ(n) = τ(m), by sieving up to a fixed LIMIT
 * and selecting the smallest n in each (σ, τ) pair that occurs more
 * than once.  The terms are stored in increasing order of σ, then τ.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Structure for sorting by sigma, then tau, then n. */
typedef struct {
    uint64_t sigma;
    uint32_t tau;
    uint64_t n;
} triple_t;

/* Static storage for the computed sequence. */
static seq_t *seq = NULL;      /* array of sequence values */
static idx_t seq_len = 0;      /* number of precomputed terms */

/* Comparator for qsort. */
static int cmp_triple(const void *p1, const void *p2)
{
    const triple_t *a = (const triple_t *)p1;
    const triple_t *b = (const triple_t *)p2;
    if (a->sigma != b->sigma)
        return (a->sigma < b->sigma) ? -1 : 1;
    if (a->tau != b->tau)
        return (a->tau < b->tau) ? -1 : 1;
    if (a->n < b->n) return -1;
    if (a->n > b->n) return 1;
    return 0;
}

/* Pre‑compute the sequence up to a fixed limit. */
static void precompute(void)
{
    const idx_t LIMIT = 2000000ULL;   /* should produce >10000 terms */

    /* Allocate buffers for sigma and tau. */
    uint64_t *sigma = (uint64_t*)calloc((size_t)LIMIT + 1, sizeof(uint64_t));
    uint32_t *tau   = (uint32_t*)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
    if (!sigma || !tau) abort();

    /* Divisor sieve: compute σ(n) and τ(n) for all n ≤ LIMIT. */
    for (idx_t i = 1; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i) {
            sigma[j] += i;
            tau[j]   += 1;
        }
    }

    /* Build array of triples (σ, τ, n). */
    triple_t *trips = (triple_t*)malloc((size_t)LIMIT * sizeof(triple_t));
    if (!trips) abort();
    for (idx_t n = 1; n <= LIMIT; ++n) {
        trips[n - 1].sigma = sigma[n];
        trips[n - 1].tau   = tau[n];
        trips[n - 1].n     = n;
    }

    free(sigma);
    free(tau);

    /* Sort triples by σ, then τ, then n. */
    qsort(trips, (size_t)LIMIT, sizeof(triple_t), cmp_triple);

    /* Collect qualifying numbers: minimal n in each duplicate group. */
    seq_t *qual = NULL;
    size_t qual_cap = 1024;
    size_t qual_len = 0;
    qual = (seq_t*)malloc(qual_cap * sizeof(seq_t));
    if (!qual) abort();

    for (idx_t i = 0; i < LIMIT; ) {
        idx_t j = i + 1;
        while (j < LIMIT &&
               trips[j].sigma == trips[i].sigma &&
               trips[j].tau   == trips[i].tau)
            ++j;

        if ((j - i) >= 2) {            /* group has at least two elements */
            uint64_t min_n = trips[i].n;
            for (idx_t k = i + 1; k < j; ++k)
                if (trips[k].n < min_n) min_n = trips[k].n;

            if (qual_len == qual_cap) {
                qual_cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(qual, qual_cap * sizeof(seq_t));
                if (!tmp) abort();
                qual = tmp;
            }
            qual[qual_len++] = (seq_t)min_n;
        }
        i = j;
    }

    free(trips);

    /* Store results in static array. */
    seq = (seq_t*)malloc(qual_len * sizeof(seq_t));
    if (!seq) abort();
    for (size_t i = 0; i < qual_len; ++i)
        seq[i] = qual[i];

    free(qual);
    seq_len = (idx_t)qual_len;
}

/* ------------------------------------------------------------ */
/*  OEIS function: a(n)                                           */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!seq) precompute();           /* lazy initialization */
    if (n == 0 || n > seq_len) return 0; /* out of bounds */
    return seq[n - 1];                /* 1‑based indexing */
}
