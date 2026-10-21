/*
 * A211656  Numbers k such that the value of sigma(k) is unique; sigma(k) =
 *          A000203(k) = sum of divisors of k.
 * https://oeis.org/A211656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 9872, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A211656: numbers k such that sigma(k) is unique.
   The routine builds from scratch a sieve of σ up to a generous
   upper bound (2·10⁶).  For each value of σ it records its multiplicity
   and then collects all k whose σ occurs exactly once.  The array of
   unique k's is stored statically and reused for any subsequent calls.
   The upper bound is chosen large enough that the first 10 000 terms
   (and a lot more) are easily obtained.  The construction is O(N log N). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Build the sequence once and keep it for later requests. */
static bool  initialized = false;
static seq_t *seq          = NULL;
static size_t seq_len      = 0;
static size_t seq_cap      = 0;

/* Sieve limit – large enough to cover many terms. */
#define LIMIT 2000000U

static void build_sequence(void)
{
    if (initialized) return;

    /* Allocate array for divisors sum σ(k). */
    uint64_t *sigma = (uint64_t *)calloc(LIMIT + 1, sizeof(uint64_t));
    if (!sigma) return;  /* allocation failure – abort silently */

    /* Sieve to compute σ(k). */
    uint64_t i, j;
    uint64_t max_sigma = 0;
    for (i = 1; i <= LIMIT; ++i) {
        for (j = i; j <= LIMIT; j += i) {
            sigma[j] += i;
        }
    }
    /* Find maximal σ value. */
    for (i = 1; i <= LIMIT; ++i) {
        if (sigma[i] > max_sigma) max_sigma = sigma[i];
    }

    /* Count occurrences of each σ value (uint8_t sufficient). */
    uint8_t *cnt = (uint8_t *)calloc(max_sigma + 1, sizeof(uint8_t));
    if (!cnt) { free(sigma); return; }

    for (i = 1; i <= LIMIT; ++i) {
        uint64_t s = sigma[i];
        if (cnt[s] < 2) cnt[s]++;   /* only need to know 0,1,>1 */
    }

    /* Allocate initial capacity for sequence array. */
    seq_cap = 256;
    seq     = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq) { free(sigma); free(cnt); return; }

    /* Collect numbers with unique σ. */
    for (i = 1; i <= LIMIT; ++i) {
        if (cnt[sigma[i]] == 1) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)i;
        }
    }

    /* Clean up temporary arrays. */
    free(sigma);
    free(cnt);

    initialized = true;
}

/*------------------------------------------------------------------*/
/* Public function implementing the OEIS entry                      */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n <= 0 */

    build_sequence();
    if (n <= (idx_t)seq_len) return seq[n - 1];
    /* Requested index beyond precomputed range – return 0. */
    return 0;
}
