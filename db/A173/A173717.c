/*
 * A173717  Sum of n mod m, summed over semiprimes m = 4, 6, 9, ..., smallest
 *          semiprime >= n.
 * https://oeis.org/A173717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A173717 – Sum of n mod m over all semiprimes m ≤ s
   where s is the smallest semiprime ≥ n.  Implementation uses a
   precomputed list of semiprimes up to a fixed limit.  For any
   n ≤ LIMIT the result is computed in O(#semiprimes ≤ s).  The
   routine is thread‑safe only with respect to the static state
   for initialization; multiple invocations with arbitrary ordering
   of n are supported. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* static tables – lazily built on first call */
static uint32_t *spf       = NULL;  /* smallest prime factor for each number */
static uint32_t *semiprimes = NULL; /* ascending list of semiprimes */
static size_t    semiprime_count = 0;
static idx_t     limit = 0;          /* upper bound for precomputation */
static bool      init_done = false;

/* ------------------------------------------------------------------ */
/*  Build tables up to at least 'up_to' (inclusive).  The function
    may be called multiple times; tables are grown if necessary.       */
static void ensure_limit(idx_t up_to)
{
    if (init_done && up_to <= limit)
        return;             /* already have enough */

    /* free old tables if reallocating */
    if (spf)   free(spf);
    if (semiprimes) free(semiprimes);

    /* round up to a convenient multiple for sieve density */
    const idx_t OVERSHOOT = 2000;          /* cushion for small n */
    limit = up_to + OVERSHOOT;

    /* allocate spf array */
    spf = (uint32_t*)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (!spf) return; /* allocation failure – unlikely in test harness */

    /* sieve for smallest prime factor */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[(size_t)i] == 0) {
            spf[(size_t)i] = (uint32_t)i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (spf[(size_t)j] == 0)
                        spf[(size_t)j] = (uint32_t)i;
                }
            }
        }
    }

    /* estimate number of semiprimes → allocate buffer */
    size_t est = (size_t)(limit / 4) + 10;
    semiprimes = (uint32_t*)malloc(est * sizeof(uint32_t));
    if (!semiprimes) return;

    /* collect semiprimes */
    size_t pos = 0;
    for (idx_t num = 4; num <= limit; ++num) {
        idx_t x = num;
        int   count = 0;
        while (x > 1 && count <= 2) {
            uint32_t p = spf[(size_t)x];
            while (x % p == 0) {
                x /= p;
                ++count;
                if (count > 2) break;
            }
        }
        if (count == 2) {
            if (pos == est) {          /* grow buffer if needed */
                est = est * 3 / 2 + 10;
                uint32_t *tmp = (uint32_t*)realloc(semiprimes, est * sizeof(uint32_t));
                if (!tmp) return;      /* allocation failure */
                semiprimes = tmp;
            }
            semiprimes[pos++] = (uint32_t)num;
        }
    }
    semiprime_count = pos;
    semiprimes = (uint32_t*)realloc(semiprimes, semiprime_count * sizeof(uint32_t));

    init_done = true;
}

/* ------------------------------------------------------------------ */
/*  Sequence function                                                  */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;         /* offset begins at 1 */

    /* Ensure tables are large enough for this n. */
    ensure_limit(n);

    /* binary search for first semiprime ≥ n */
    size_t low = 0, high = semiprime_count;
    while (low < high) {
        size_t mid = (low + high) >> 1;
        if ((idx_t)semiprimes[mid] < n)
            low = mid + 1;
        else
            high = mid;
    }
    if (low == semiprime_count)   /* should not happen with ample limit */
        return (seq_t)0;

    /* sum of n mod m over all semiprimes ≤ that one */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i <= low; ++i) {
        sum += (seq_t)n % (seq_t)semiprimes[i];
    }
    return sum;
}
