/*
 * A094142  Smallest semiperimeter of primitive Pythagorean triangle whose
 *          legs differ by A058529(n).
 * https://oeis.org/A094142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 34, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generates all primitive Pythagorean triples up to a
   pre‑defined bound, extracts the minimal semiperimeter for each
   distinct leg difference d = |m^2 − 2*m*n − n^2|, sorts the results by d,
   and returns the n‑th smallest such semiperimeter. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* compute gcd of two uint64_t values */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

typedef struct {
    uint64_t d;   /* leg difference */
    seq_t   s;    /* semiperimeter */
} Pair;

static int cmp_pairs(const void *a, const void *b)
{
    const Pair *pa = (const Pair *)a;
    const Pair *pb = (const Pair *)b;
    if (pa->d < pb->d) return -1;
    if (pa->d > pb->d) return 1;
    return 0;
}

/* static caching area */
static seq_t *cache    = NULL;   /* sorted array of minimal semicircumferences */
static size_t cache_len = 0;     /* number of elements in cache */
static bool initialized = false;

/* initialize cache once */
static void init_sequence(void)
{
    if (initialized) return;

    /* bound on m; enough to generate at least 10000 terms */
    const unsigned int M_LIMIT = 4000;

    /* dynamic array for all (m,n) pairs */
    size_t pairs_cap = 0;
    size_t pairs_len = 0;
    Pair *pairs = NULL;

    for (unsigned int m = 2; m <= M_LIMIT; ++m) {
        for (unsigned int n = 1; n < m; ++n) {
            /* opposite parity and coprime */
            if ((m & 1) == (n & 1)) continue;
            if (gcd_u64(m, n) != 1) continue;

            uint64_t mm = (uint64_t)m * m;
            uint64_t nn = (uint64_t)n * n;
            uint64_t two_mn = 2ULL * m * n;
            uint64_t d;
            if (mm >= two_mn + nn) d = mm - two_mn - nn;
            else                   d = two_mn + nn - mm;

            seq_t s = (seq_t)m * ((seq_t)m + (seq_t)n);

            if (pairs_len == pairs_cap) {
                pairs_cap = pairs_cap ? pairs_cap * 2 : 1024;
                pairs = (Pair *)realloc(pairs, pairs_cap * sizeof(Pair));
            }
            pairs[pairs_len].d = d;
            pairs[pairs_len].s = s;
            ++pairs_len;
        }
    }

    /* sort by difference */
    qsort(pairs, pairs_len, sizeof(Pair), cmp_pairs);

    /* count distinct differences to size cache */
    size_t distinct = 0;
    for (size_t i = 0; i < pairs_len; ++i) {
        if (i == 0 || pairs[i].d != pairs[i - 1].d) ++distinct;
    }

    cache = (seq_t *)malloc(distinct * sizeof(seq_t));
    cache_len = 0;

    /* scan sorted pairs, retain minimal s for each distinct d */
    uint64_t current_d = 0;
    seq_t min_s = 0;
    for (size_t i = 0; i < pairs_len; ++i) {
        if (i == 0 || pairs[i].d != current_d) {
            if (i != 0) cache[cache_len++] = min_s;  /* store previous d */
            current_d = pairs[i].d;
            min_s = pairs[i].s;
        } else {
            if (pairs[i].s < min_s) min_s = pairs[i].s;
        }
    }
    if (pairs_len > 0) cache[cache_len++] = min_s;  /* last group */

    free(pairs);
    initialized = true;
}

/* public function */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0 || n > cache_len) return (seq_t)0;  /* undefined, safe */
    return cache[n - 1];
}
