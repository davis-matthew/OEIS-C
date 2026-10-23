/*
 * A185653  Expansion of exp( Sum_{n>=1} -3*sigma(2n)*x^n/n ) in powers of x.
 * https://oeis.org/A185653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 37, i32 n <= 1024, i64 n <= 1024,
 *            i128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A185653
 * Offset 0
 * Recurrence:
 *   a(0) = 1
 *   n * a(n) = -3 * sum_{k=1..n} sigma(2k) * a(n-k)   (n >= 1)
 *
 * Where sigma(m) is the sum of all positive divisors of m.
 * The implementation uses memoised dynamic arrays and a simple
 * divisor–sum sieve.  All arithmetic is performed with the
 * mandatory integer type seq_t.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;             /* cache of a(n) values */
static size_t   a_cap  = 0;               /* current size of a_cache */

/* Cache for divisor sums sigma(1 .. 2*max_n) */
static uint64_t *sigma_cache = NULL;
static size_t   sigma_cap  = 0;           /* maximum index for sigma_cache */

/* ------------------------------------------------------------------ */
/* Helper: extend a_cache up to index n (inclusive)                  */
static void extend_a_cache(idx_t n)
{
    size_t old_cap = a_cap;
    size_t new_cap = (size_t)n + 1;

    if (new_cap > a_cap) {
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                     /* allocation failure */
        a_cache = tmp;
        a_cap = new_cap;
    }

    /* Compute new entries a(k) for k = old_cap .. n */
    for (size_t k = old_cap; k <= (size_t)n; ++k) {
        if (k == 0) {
            a_cache[0] = 1;                    /* a(0) = 1 */
            continue;
        }

        seq_t sum = 0;
        for (size_t j = 1; j <= k; ++j) {
            /* -3 * sigma(2*j) as seq_t */
            seq_t c = -(seq_t)3 * (seq_t)(sigma_cache[2 * j]);
            sum += a_cache[k - j] * c;
        }
        a_cache[k] = sum / (seq_t)k;            /* division exact */
    }
}

/* ------------------------------------------------------------------ */
/* Helper: extend sigma_cache up to index 2*n (inclusive)             */
static void extend_sigma_cache(idx_t n)
{
    size_t needed = 2 * (size_t)n;            /* need sigma(1 .. 2*n) */
    if (needed + 1 <= sigma_cap) return;      /* already large enough */

    free(sigma_cache);
    sigma_cache = (uint64_t *)malloc((needed + 1) * sizeof(uint64_t));
    if (!sigma_cache) exit(1);
    sigma_cap = needed + 1;

    for (size_t i = 1; i <= needed; ++i) sigma_cache[i] = 0;

    for (size_t d = 1; d <= needed; ++d) {
        for (size_t m = d; m <= needed; m += d) {
            sigma_cache[m] += d;               /* divisor sum */
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the test harness                       */
seq_t a(idx_t n)
{
    if (a_cache == NULL) {                   /* initialise caches */
        a_cache = (seq_t *)malloc(sizeof(seq_t)); /* size 1, a(0) */
        if (!a_cache) exit(1);
        a_cap = 1;
        a_cache[0] = 1;                       /* a(0) = 1 */
    }

    extend_sigma_cache(n);
    extend_a_cache(n);

    return a_cache[(size_t)n];
}
