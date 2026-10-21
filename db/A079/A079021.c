/*
 * A079021  Suppose p and q = p+22 are primes. Define the difference pattern
 *          of (p,q) to be the successive differences of the primes in the
 *          range p to q. There are 51 possible difference patterns, shown in
 *          the Comments line. Sequence gives smallest value of p for each
 *          difference pattern, sorted by magnitude.
 * https://oeis.org/A079021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 50, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: sieve primes up to a fixed limit, find all 51 difference patterns
   of primes in intervals of length 22, record the smallest prime p for each
   pattern, sort the list of p's, and return the nth element. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Forward declaration – seq_t and idx_t are provided by the build. */
seq_t a(idx_t n);

/* Internal data structures and static state. */
typedef struct {
    uint8_t  len;          /* number of differences */
    uint8_t  diffs[12];    /* the differences (max 11, extra byte for safety) */
    seq_t    first_p;      /* smallest p that produced this pattern */
} pattern_t;

static bool          seq_initialized = false;
static seq_t         sorted_p[51];
static idx_t         sorted_len = 0;

/* --------------------------------------------------------------------- */
static void build_sequence(void)
{
    /* Sieve limit: large enough to contain all 51 patterns. */
    /* 10 000 000 gives ~664 579 primes – comfortably above all needed.   */
    const idx_t LIMIT = 10000000ULL;

    /* ---------------------------------------------------------------
       1. Generate prime sieve.
       --------------------------------------------------------------- */
    uint8_t *is_prime = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
    if (!is_prime) return;                     /* allocation failure */

    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    if (LIMIT >= 0) is_prime[0] = 0;
    if (LIMIT >= 1) is_prime[1] = 0;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* ---------------------------------------------------------------
       2. Count primes to allocate array.
       --------------------------------------------------------------- */
    idx_t prime_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;

    idx_t *primes = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    if (!primes) {
        free(is_prime);
        return;
    }

    idx_t k = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[k++] = i;

    /* ---------------------------------------------------------------
       3. Discover patterns.
       --------------------------------------------------------------- */
    pattern_t patterns[51];
    idx_t pattern_count = 0;

    for (idx_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];

        if (p + 22ULL > LIMIT)           /* further primes would exceed limit */
            break;

        if (!is_prime[p + 22ULL])        /* q not prime – skip */
            continue;

        /* Find first index after q. */
        idx_t j = i + 1;
        while (j < prime_cnt && primes[j] <= p + 22ULL) ++j;

        /* primes[i .. j-1] are within [p, q]. */
        idx_t len_primes = j - i;          /* count of primes in interval */
        if (len_primes < 2) continue;      /* should not happen */

        uint8_t diffs[12];
        idx_t diff_len = len_primes - 1;
        for (idx_t t = 0; t < diff_len; ++t)
            diffs[t] = (uint8_t)(primes[i + t + 1] - primes[i + t]);

        /* Check if this pattern is new. */
        bool found = false;
        for (idx_t t = 0; t < pattern_count; ++t) {
            if (patterns[t].len != diff_len) continue;
            if (memcmp(patterns[t].diffs, diffs, diff_len) == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            patterns[pattern_count].len = (uint8_t)diff_len;
            memcpy(patterns[pattern_count].diffs, diffs, diff_len);
            patterns[pattern_count].first_p = (seq_t)p;
            ++pattern_count;
            if (pattern_count == 51) break;   /* all patterns collected */
        }
    }

    free(is_prime);
    free(primes);

    /* ---------------------------------------------------------------
       4. Sort patterns by their first_p.
       --------------------------------------------------------------- */
    for (idx_t i = 0; i < pattern_count; ++i) {
        for (idx_t j = i + 1; j < pattern_count; ++j) {
            if (patterns[i].first_p > patterns[j].first_p) {
                pattern_t tmp = patterns[i];
                patterns[i] = patterns[j];
                patterns[j] = tmp;
            }
        }
    }

    /* ---------------------------------------------------------------
       5. Copy to global sorted array.
       --------------------------------------------------------------- */
    for (idx_t i = 0; i < pattern_count; ++i)
        sorted_p[i] = patterns[i].first_p;
    sorted_len = pattern_count;
    seq_initialized = true;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!seq_initialized)
        build_sequence();

    if (n == 0 || n > sorted_len)
        return 0;   /* undefined beyond the 51 known patterns */

    return sorted_p[n - 1];
}
