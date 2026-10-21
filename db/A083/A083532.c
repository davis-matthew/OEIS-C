/*
 * A083532  First difference sequence of A007369. Differences between
 *          impossible values for sum of divisors of n.
 * https://oeis.org/A083532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007369(n+1) - A007369(n) A007369: numbers that are NOT
 *            attainable as σ(k) for any k. a(n) is the difference between
 *            consecutive terms of A007369. The implementation precomputes σ
 *            values up to a fixed limit, records unreachable sigma values
 *            (nonsense values), and stores the differences bet...
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Compile‑time constants ---------------------------------------------------- */
#define LIMIT       2000000      /* largest k for which σ(k) is computed   */
#define MAX_SIGMA   12000000     /* maximum σ(k) expected for k <= LIMIT   */

/* Global tables ------------------------------------------------------------ */
static uint32_t spf[LIMIT + 1];          /* smallest prime factor   */
static seq_t   sigma[LIMIT + 1];         /* divisor sums            */
static unsigned char reachable[MAX_SIGMA + 1];   /* 0/1 flag for σ values  */
static seq_t *diffs = NULL;              /* differences of A083532 */
static size_t diff_len = 0;              /* number of entries in diffs */

/* One‑time initialization ----------------------------------------------- */
static void init(void)
{
    /* 1. Compute smallest prime factor (spf) table ---------------------------------- */
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }

    /* 2. Compute σ values ---------------------------------------------------------- */
    sigma[1] = 1;
    size_t max_sigma = 1;
    for (uint32_t n = 2; n <= LIMIT; ++n) {
        uint32_t p = spf[n];
        uint32_t m = n;
        uint32_t e = 0;
        while (m % p == 0) { m /= p; ++e; }

        seq_t part = 1;                  /* (p^(e+1)-1)/(p-1) */
        seq_t pow  = 1;
        for (uint32_t k = 0; k < e; ++k) {
            pow  *= (seq_t)p;
            part += pow;
        }
        sigma[n] = sigma[m] * part;

        if ((size_t)(uint64_t)sigma[n] > max_sigma)
            max_sigma = (size_t)(uint64_t)sigma[n];
    }

    /* 3. Mark reachable σ values ------------------------------------------------- */
    memset(reachable, 0, sizeof(reachable));
    for (uint32_t n = 1; n <= LIMIT; ++n) {
        size_t val = (size_t)(uint64_t)sigma[n];
        if (val <= MAX_SIGMA)
            reachable[val] = 1;
    }

    /* 4. Count nonsense values and allocate differences array ---------------------- */
    size_t nonsense_cnt = 0;
    for (size_t v = 1; v <= max_sigma; ++v)
        if (!reachable[v]) ++nonsense_cnt;

    diff_len = (nonsense_cnt > 0) ? nonsense_cnt - 1 : 0;
    diffs    = (seq_t *)malloc(diff_len * sizeof(seq_t));

    /* 5. Store differences between successive nonsense values ----------------------- */
    if (diff_len > 0) {
        bool seen = false;
        seq_t prev_val = 0;
        size_t idx = 0;
        for (size_t v = 1; v <= max_sigma; ++v) {
            if (!reachable[v]) {
                if (!seen) {
                    prev_val = (seq_t)v;
                    seen = true;
                } else {
                    diffs[idx++] = (seq_t)v - prev_val;
                    prev_val = (seq_t)v;
                }
            }
        }
    }
}

/* Public function -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure initialization is performed once ----------------------------------- */
    static bool initialized = false;
    if (!initialized) {
        init();
        initialized = true;
    }

    /* Safety: if the precomputed range does not cover n, it is an error.
               In practice n will be within the supported range. */
    if ((size_t)n == 0 || (size_t)n > diff_len)
        return (seq_t)0;            /* out of bounds → return zero (undefined) */

    return diffs[(size_t)n - 1];
}
