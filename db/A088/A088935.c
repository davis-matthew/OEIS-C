/*
 * A088935  Numbers n such that leading digits of 2^n and 5^n are equal.
 * https://oeis.org/A088935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 3000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088935: exponents n with equal leading digits of 2^n and 5^n.
   The algorithm scans n sequentially and keeps all n for which the
   fractional part of n*log10(2) lies in (log10(3), 1-log10(3)).  The
   first term a(1) is 0.  Subsequent terms are added on demand. */

#include <stdlib.h>

static const unsigned long long SCALE      = 1000000000000ULL;  /* 1e12 */
static const unsigned long long LOG2_VAL   = 301029995664ULL;    /* log10(2) * 1e12 rounded */
static const unsigned long long LOG3_LOW  = 477121254719ULL;    /* log10(3) * 1e12 rounded */
static const unsigned long long LOG3_HIGH = 522878745281ULL;    /* (1-log10(3)) * 1e12 rounded */

#define MAX_TERMS 200000   /* sufficient for n up to about 10⁴ */

static seq_t term_cache[MAX_TERMS];
static size_t term_count     = 0;
static idx_t  last_n_scanned = 0;
static int    initialized   = 0;

/* Append a new term to the cache if space allows */
static void add_term(idx_t n_val)
{
    if (term_count >= MAX_TERMS) return;      /* safety guard */
    term_cache[term_count++] = (seq_t)n_val;
}

/* Return the n‑th term of A088935 (a(1) = 0).  The function is
   memoised: it remembers previously generated terms and continues
   generating new ones on demand. */
seq_t a(idx_t n)
{
    if (!initialized) {
        term_cache[0] = (seq_t)0;   /* a(1) = 0 */
        term_count = 1;
        last_n_scanned = 0;
        initialized = 1;
    }

    while (term_count < n) {
        last_n_scanned++;
        unsigned long long prod  = (unsigned long long)last_n_scanned * LOG2_VAL;
        unsigned long long frac  = prod % SCALE;            /* fractional part scaled */
        if (frac > LOG3_LOW && frac < LOG3_HIGH) {
            add_term(last_n_scanned);
        }
    }
    return term_cache[n - 1];
}
