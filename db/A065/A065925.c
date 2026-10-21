/*
 * A065925  Smallest k such that sopf(n+k) = sopf(k), where sopf = A008472.
 * https://oeis.org/A065925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A065925
   a(n) = smallest k >= 1 such that
          sum of distinct prime factors of (n + k)
          equals sum of distinct prime factors of k.
   The sum of distinct prime factors is the OEIS A016913. */

/* #include <stdbool.h> is required for the bool type used below. */
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *spf = NULL;   /* smallest prime factor for each integer */
static seq_t *sdp = NULL;      /* sum of distinct prime factors */
static size_t sieve_limit = 0; /* current maximum index computed */

/* ------------------------------------------------------------------ */
/* Helper: compute spf and sdp arrays up to a given limit using a linear
   sieve.  The arrays are allocated (or reallocated) and filled.        */
static void compute_sieve(size_t new_limit) {
    /* Free previous arrays if any */
    free(spf);
    free(sdp);

    /* Allocate new arrays (one extra for 0..new_limit) */
    spf = (uint32_t *)malloc((new_limit + 1) * sizeof(uint32_t));
    sdp = (seq_t *)malloc((new_limit + 1) * sizeof(seq_t));
    if (!spf || !sdp) { /* in a real program handle allocation failure */
        return;
    }

    spf[0] = 0;
    if (new_limit >= 1) spf[1] = 1;
    sdp[0] = 0;
    if (new_limit >= 1) sdp[1] = 0;

    /* Linear sieve to fill spf */
    size_t primes_count = 0;
    uint32_t *primes = (uint32_t *)malloc((new_limit / 10 + 10) * sizeof(uint32_t));
    if (!primes) return; /* insufficient memory, stop */

    for (size_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            primes[primes_count++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_count; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)p * (uint64_t)i;
            if (prod > new_limit) break;
            spf[prod] = p;
            if (p == spf[i]) break;
        }
    }
    free(primes);

    /* Compute sum of distinct prime factors (sdp) */
    for (size_t i = 2; i <= new_limit; ++i) {
        uint32_t p = spf[i];
        size_t rest = i / p;
        if (spf[rest] == p) {
            /* duplicate prime; do not add again */
            sdp[i] = sdp[rest];
        } else {
            sdp[i] = sdp[rest] + (seq_t)p;
        }
    }

    sieve_limit = new_limit;
}

/* ------------------------------------------------------------------ */
/* Ensure that the sieve arrays cover indices up to 'need'.           */
static void ensure_limit(idx_t need) {
    if ((size_t)need <= sieve_limit) return;
    /* Grow the limit (double it or at least enough for 'need') */
    size_t new_limit = sieve_limit * 2 + 1000;
    if (new_limit < (size_t)need) new_limit = (size_t)need + 1000;
    compute_sieve(new_limit);
}

/* ------------------------------------------------------------------ */
/* One‑time initializer for the sieve data.                           */
static void init_once(void) {
    static bool inited = false;
    if (!inited) {
        compute_sieve(2000000); /* small starting table; grows if needed */
        inited = true;
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                           */
seq_t a(idx_t n) {
    init_once();                       /* ensure tables are ready */

    if (n == 0) return 0;              /* safety for offset 1 */

    /* Search for the smallest k such that sdp(k) == sdp(n + k) */
    for (idx_t k = 1; ; ++k) {
        idx_t idx = n + k;
        ensure_limit(idx);             /* grow tables if needed */

        /* The arrays are now guaranteed big enough */
        if (sdp[k] == sdp[idx]) {
            return (seq_t)k;
        }
    }
}
