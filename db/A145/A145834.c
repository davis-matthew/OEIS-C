/*
 * A145834  a(n) = difference between the n-th composite number and the sum of
 *          its prime factors with repetition.
 * https://oeis.org/A145834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002808(n) – sum of prime factors (with multiplicity)
 * Range:     u8 n <= 219, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A002808(n) is the n‑th composite number.  */
/*   Computes seq_t a(idx_t n) for any n ≥ 1. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

static uint64_t *spf = NULL;          /* smallest prime factors */
static uint64_t *composites = NULL;   /* list of composite numbers */
static size_t   comp_len = 0;         /* number of composites stored */
static uint64_t cur_limit = 0;        /* current sieve limit */

/* Rebuild the sieve and composite list up to 'limit'. */
static void rebuild(uint64_t limit) {
    if (spf) free(spf);
    build_spf(limit, (idx_t **)&spf);           /* spf[0..limit] initialized */

    /* Count composites (>=4 and spf[i] != i). */
    size_t count = 0;
    for (uint64_t i = 4; i <= limit; ++i) {
        if (spf[i] != i) ++count;
    }

    if (composites) free(composites);
    composites = (uint64_t *)malloc(count * sizeof(uint64_t));
    comp_len = count;
    size_t pos = 0;
    for (uint64_t i = 4; i <= limit; ++i) {
        if (spf[i] != i) composites[pos++] = i;
    }
    cur_limit = limit;
}

/* Ensure that the composite list contains at least 'n' entries. */
static void ensure_limit(idx_t n) {
    if ((size_t)n <= comp_len) return;
    if (!cur_limit) rebuild(4);
    while ((size_t)n > comp_len) rebuild(cur_limit * 2);   /* double until enough */
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety: OEIS offset is 1 */
    ensure_limit(n);

    uint64_t c = composites[n - 1];      /* nth composite */
    seq_t sum = 0;
    uint64_t m = c;
    while (m > 1) {                      /* add prime factors with multiplicity */
        sum += (seq_t)spf[m];
        m   /= spf[m];
    }
    return (seq_t)c - sum;
}
