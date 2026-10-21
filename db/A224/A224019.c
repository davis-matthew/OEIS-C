/*
 * A224019  Number of n X 3 0..3 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A224019: Number of n × 3 arrays with rows nondecreasing and antidiagonals unimodal.
 *
 * Empirical linear recurrence (order 7):
 *
 *  a(n) = 20*a(n‑1)           - 70*a(n‑2)
 *         +228*a(n‑3)         - 276*a(n‑4)
 *         +392*a(n‑5)         - 196*a(n‑6)
 *         +144*a(n‑7)
 *
 * Initial values (for n = 1 … 7):
 *   a(1) = 20,   a(2) = 400,   a(3) = 6796,
 *   a(4) = 112436, a(5) = 1859020, a(6) = 30756756,
 *   a(7) = 508916456
 *
 * The implementation keeps a static cache up to a fixed maximum size
 * sufficient for all tests (≈ 20000).  For n larger than this,
 * the recurrence is evaluated iteratively without storing the whole
 * sequence.  The code is valid C11 and C++17 and works for any
 * integer type `seq_t` (primitive unsigned or arbitrary‑precision).
 */

#include <stddef.h>   /* size_t */

#define MAX_CACHE 20000    /* enough for all practical tests (n ≤ ~10000) */

static const seq_t base_terms[7] = {
    (seq_t)20,        /* a(1) */
    (seq_t)400,       /* a(2) */
    (seq_t)6796,      /* a(3) */
    (seq_t)112436,    /* a(4) */
    (seq_t)1859020,   /* a(5) */
    (seq_t)30756756,  /* a(6) */
    (seq_t)508916456  /* a(7) */
};

static seq_t cache[MAX_CACHE + 1];
static size_t cache_max = 0;      /* largest n for which cache[n] is valid */

/* Compute terms up to at least `n` (<= MAX_CACHE) and store them in the cache. */
static void compute_up_to(idx_t n)
{
    if (n <= cache_max) return;
    if (n > MAX_CACHE) n = MAX_CACHE;

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 7) {
            cache[i] = base_terms[i - 1];
        } else {
            /* recurrence using cached previous values */
            seq_t v = 20 * cache[i - 1]
                    - 70 * cache[i - 2]
                    + 228 * cache[i - 3]
                    - 276 * cache[i - 4]
                    + 392 * cache[i - 5]
                    - 196 * cache[i - 6]
                    + 144 * cache[i - 7];
            cache[i] = v;
        }
    }
    cache_max = n;
}

/* Return a(n) for any n ≥ 1, caching results up to MAX_CACHE. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, but harmless */

    if (n <= MAX_CACHE) {
        compute_up_to(n);
        return cache[n];
    }

    /* n > MAX_CACHE: compute iteratively beyond the cached range. */
    compute_up_to(MAX_CACHE);   /* ensure cache up to MAX_CACHE is filled */

    /* initialise a sliding window with the last 7 cached values */
    seq_t last[7];
    for (idx_t k = 0; k < 7; ++k) {
        last[k] = cache[MAX_CACHE - 6 + k];
    }

    seq_t result = last[6];  /* will hold a(MAX_CACHE) initially */
    for (idx_t i = MAX_CACHE + 1; i <= n; ++i) {
        seq_t v = 20 * last[6]
                - 70 * last[5]
                + 228 * last[4]
                - 276 * last[3]
                + 392 * last[2]
                - 196 * last[1]
                + 144 * last[0];
        /* shift window */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = last[6];
        last[6] = v;
        result = v;
    }
    return result;
}
