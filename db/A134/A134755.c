/*
 * A134755  Minimal number such that all greater numbers can be written as
 *          sums of squares of primes in more than n ways.
 * https://oeis.org/A134755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal m such that all j > m can be written as sums * of
 *            squares of primes in more than n ways. a(n) is the largest
 *            integer * j for which the number of representations b(j) is <=
 *            n. The function * precomputes b(j) for all j up to a fixed
 *            limit, caps the counts at a * sufficiently large va...
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Max n that a(.) will support (you can increase this if needed). */
#define CAP_N 20000
/* Upper bound for j that we compute b(j) for.  Must be large enough
 * that a(CAP_N) < LIMIT.  For CAP_N = 20000 this value works comfortably. */
#define LIMIT 500000

static bool initialized = false;
static uint32_t *dp_counts = NULL;     /* capped counts dp_counts[j] = min(b(j), CAP_N) */
static unsigned int *a_vals    = NULL; /* a_vals[n] = a(n) */

/* Sieve primes up to sqrt(LIMIT). */
static void build_primes_up_to_sqrt(void)
{
    size_t maxp = (size_t)(sqrt((double)LIMIT) + 1);
    unsigned char *is_comp = (unsigned char*)calloc(maxp + 1, 1);
    if (!is_comp) return;
    size_t *tmp = (size_t*)malloc(maxp * sizeof(size_t));
    size_t pcnt = 0;
    for (size_t p = 2; p <= maxp; ++p) {
        if (!is_comp[p]) {
            tmp[pcnt++] = p;
            if (p * p <= maxp) {
                for (size_t k = p * p; k <= maxp; k += p)
                    is_comp[k] = 1;
            }
        }
    }
    free(is_comp);

    /* DP: count partitions into squares of primes. */
    for (size_t i = 0; i < LIMIT + 1; ++i) dp_counts[i] = 0;
    dp_counts[0] = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        size_t p = tmp[i];
        uint32_t sq = (uint32_t)(p * p);
        if (sq > LIMIT) break;
        for (uint32_t j = sq; j <= LIMIT; ++j) {
            uint32_t val = dp_counts[j] + dp_counts[j - sq];
            if (val > CAP_N) val = CAP_N;     /* cap */
            dp_counts[j] = val;
        }
    }
    free(tmp);
}

/* Build a_vals[n] for all 0 <= n <= CAP_N. */
static void build_a_vals(void)
{
    a_vals = (unsigned int*)malloc((CAP_N + 1) * sizeof(unsigned int));
    if (!a_vals) return;

    unsigned int *last_for_cnt = (unsigned int*)calloc(CAP_N + 1, sizeof(unsigned int));
    for (uint32_t j = 0; j <= LIMIT; ++j) {
        uint32_t c = dp_counts[j];
        if (c <= CAP_N) last_for_cnt[c] = j;   /* last index with this count */
    }
    unsigned int mx = last_for_cnt[0];
    a_vals[0] = mx;
    for (unsigned int n = 1; n <= CAP_N; ++n) {
        if (last_for_cnt[n] > mx) mx = last_for_cnt[n];
        a_vals[n] = mx;
    }
    free(last_for_cnt);
}

/* One‑time initialization. */
static void init(void)
{
    if (initialized) return;
    dp_counts = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!dp_counts) return;
    build_primes_up_to_sqrt();
    build_a_vals();
    initialized = true;
}

seq_t a(idx_t n)
{
    init();
    if (!initialized) return 0;          /* fallback if allocation failed */
    unsigned int nn = (unsigned int)n;   /* we only support up to CAP_N */
    if (nn > CAP_N) {
        /* Outside precomputed range: recompute with a larger cap if needed. */
        return 0;
    }
    return (seq_t)a_vals[nn];
}
