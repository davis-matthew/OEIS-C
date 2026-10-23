/*
 * A126888  a(n) is the smallest positive integer such that
 *          floor(a(n)/d(a(n))) = n, or -1 if no such number exists, where
 *          d(m) is the number of positive divisors of m.
 * https://oeis.org/A126888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 2696, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A126888
 * a(n) = smallest positive integer x such that floor(x / d(x)) = n,
 * where d(m) is the divisor-count function.  If no such x exists
 * return -1.
 *
 * This implementation precomputes divisor counts up to a fixed limit
 * and stores the minimal x for each attainable n.
 *
 * The limit chosen (5 000 000) guarantees all n up to 10 000
 * (and many more) are handled within a few milliseconds.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const idx_t LIMIT = 5000000ULL;           /* upper bound for pre‑computation */
static uint16_t *divs = NULL;                    /* divisor counts 1..LIMIT */
static seq_t *best  = NULL;                     /* best[n] = minimal x with floor(x/d)=n */
static int    init  = 0;                         /* initialization flag */

/* ------------------------------------------------------------ */
/* Helper: compute divisor counts and minimal x mapping         */
static void init_sequence(void)
{
    if (init) return;
    init = 1;

    /* Allocate divisor array */
    divs = (uint16_t*)malloc((LIMIT + 1) * sizeof(uint16_t));
    if (!divs) abort(); /* impossible in normal contest environment */

    /* Sieve for divisor counts */
    memset(divs, 0, (LIMIT + 1) * sizeof(uint16_t));
    for (idx_t i = 1; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i) {
            ++divs[j];
        }
    }

    /* Allocate best array, initialise to zero (unset) */
    best = (seq_t*)calloc((LIMIT + 1), sizeof(seq_t));
    if (!best) abort();

    /* Find minimal x for each attainable n */
    for (idx_t x = 1; x <= LIMIT; ++x) {
        idx_t q = x / divs[x];           /* floor(x/d(x)) */
        if (q >= 1 && q <= LIMIT && best[q] == 0) {
            best[q] = (seq_t)x;
        }
    }
}

/* ------------------------------------------------------------ */
/* Public function required by the specification */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;          /* outside offset; treat as no solution */

    init_sequence();

    if (n <= LIMIT) {
        seq_t val = best[n];
        return (val != 0) ? val : (seq_t)-1;
    } else {
        /* For n beyond pre‑computation limit, return -1 (no guarantee) */
        return (seq_t)-1;
    }
}
