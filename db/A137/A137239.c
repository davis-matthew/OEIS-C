/*
 * A137239  a(n) is the number of Catalan numbers with n digits.
 * https://oeis.org/A137239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9812, u16 n <= 9812, u32 n <= 9812, u64 n <= 9812,
 *            u128 n <= 9812, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A137239: Number of Catalan numbers with n decimal digits.
   The algorithm counts Catalan numbers by increasing k and records
   the frequency of each digit length.  It stops when the next
   Catalan number would exceed the requested length. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* External integer types provided by the build system.  They are
   already typedefs, so we use them directly without redefining. */

/* ---------- Static state ---------- */
static seq_t *a_counts = NULL;           /* a_counts[n] = a(n) */
static size_t a_cap   = 0;               /* allocated size */

/* next Catalan index to process (k in C_k) */
static idx_t next_k = 0;
/* highest digit length that has been processed */
static idx_t max_digits = 0;

/* log10 factorial table: logFact[i] = log10(i!)
   allocated up to current log_cap (>= 2*next_k)   */
static long double *logFact   = NULL;
static size_t       log_cap   = 0;

/* ---------- Helper functions ---------- */

/* Grow logFact table so that it contains values up to 2*limit. */
static void
ensure_logfact(idx_t limit)
{
    size_t need = (size_t)(2 * limit + 1);
    if (log_cap == 0) {
        /* initial allocation */
        log_cap = need > 128 ? need : 128;
        logFact = (long double *)malloc(log_cap * sizeof(long double));
        if (!logFact) return; /* allocation failure – caller will fail silently */
        logFact[0] = 0.0L;
        for (size_t i = 1; i < log_cap; ++i)
            logFact[i] = logFact[i-1] + log10l((long double)i);
    } else if (log_cap < need) {
        size_t new_cap = log_cap * 2;
        if (new_cap < need) new_cap = need;
        long double *tmp = (long double *)realloc(logFact, new_cap * sizeof(long double));
        if (!tmp) return; /* allocation failure – caller will fail silently */
        for (size_t i = log_cap; i < new_cap; ++i)
            tmp[i] = tmp[i-1] + log10l((long double)i);
        logFact = tmp;
        log_cap = new_cap;
    }
}

/* Ensure a_counts array can hold up to index `target` (exclusive). */
static void
ensure_counts(idx_t target)
{
    /* Grow a_counts if necessary */
    if (a_cap <= target) {
        size_t new_cap = (a_cap == 0) ? 64 : a_cap * 2;
        if (new_cap <= target) new_cap = target + 1;
        seq_t *tmp = (seq_t *)realloc(a_counts, new_cap * sizeof(seq_t));
        if (!tmp) return; /* allocation failure – caller will fail silently */
        for (size_t i = a_cap; i < new_cap; ++i)
            tmp[i] = 0;
        a_counts = tmp;
        a_cap    = new_cap;
    }

    /* Count Catalan numbers until the digit length exceeds target. */
    while (max_digits <= target) {
        /* Ensure factorial logs are up to date */
        ensure_logfact(next_k);

        /* log10(C_{next_k}) = log10((2k)!/(k!k!)) - log10(k+1) */
        long double logC = logFact[(size_t)(2 * next_k)]
                       - 2.0L * logFact[(size_t)next_k]
                       - log10l((long double)(next_k + 1));

        idx_t digits = (idx_t)(floorl(logC) + 1.0L);

        if (digits > target) break;           /* no need to count this one */

        if (digits >= a_cap) {
            /* grow counts array for safety (should not happen often) */
            size_t new_cap = digits + 1;
            seq_t *tmp = (seq_t *)realloc(a_counts, new_cap * sizeof(seq_t));
            if (!tmp) return;
            for (size_t i = a_cap; i < new_cap; ++i)
                tmp[i] = 0;
            a_counts = tmp;
            a_cap    = new_cap;
        }

        a_counts[digits] = a_counts[digits] + 1;
        if (digits > max_digits) max_digits = digits;

        ++next_k;
    }
}

/* ---------- Public sequence function ---------- */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 0 */

    ensure_counts(n);

    return (n < a_cap) ? a_counts[n] : 0;
}
