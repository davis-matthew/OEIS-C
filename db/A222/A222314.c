/*
 * A222314  Number of characters when n-th row of Pascal's triangle is written
 *          in base 10 (including the n spaces).
 * https://oeis.org/A222314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{k=0}^n ( floor( log10( C(n,k) ) ) + 1 ) + n * where
 *            C(n,k) is the binomial coefficient. * The implementation
 *            pre‑computes the logarithm in base 10 of all factorials * using
 *            integer scaling, which guarantees exact digit counts
 * Range:     u8 n <= 32, u16 n <= 548, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>     /* log10 */

static const uint64_t SCALE = 1000000000000ULL;   /* 10^12 */
static uint64_t *log_fact = NULL;                /* scaled log10 factorials */
static size_t log_fact_cap = 0;                  /* capacity of log_fact */
static size_t log_fact_last = 0;                 /* last computed entry */

/* Expand log_fact array up to index `n`.  New entries are computed
 * using scaled integer values of log10 for each integer.  The scaling
 * and rounding are done so that the final floor operation is exact. */
static void
ensure_log_fact(idx_t n)
{
    if ((size_t)n + 1 <= log_fact_cap)
        return;

    size_t new_cap = (size_t)n + 1;
    if (new_cap < 2)
        new_cap = 2;

    uint64_t *tmp = (uint64_t *)realloc(log_fact, new_cap * sizeof(uint64_t));
    if (!tmp)
        abort();                         /* fatal, should not happen */
    log_fact = tmp;
    log_fact_cap = new_cap;

    if (log_fact_last == 0) {
        log_fact[0] = 0;                /* 0! = 1  → log10(1) = 0 */
        log_fact_last = 0;
    }

    /* Compute missing entries */
    for (size_t i = log_fact_last + 1; i <= (size_t)n; ++i) {
        long double lg = log10((long double)i);
        uint64_t scaled = (uint64_t)(lg * SCALE + 0.5L);     /* round to nearest */
        log_fact[i] = log_fact[i - 1] + scaled;
    }
    log_fact_last = (size_t)n;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    /* Prepare factorial logarithms up to n. */
    ensure_log_fact(n);

    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* log10( C(n,k) ) = log10(n!) - log10(k!) - log10((n-k)!) */
        uint64_t digits = (log_fact[n] - log_fact[k] - log_fact[n - k]) / SCALE + 1;
        total += (seq_t)digits;
    }
    /* Add n spaces between the n+1 coefficients. */
    total += (seq_t)n;
    return total;
}
