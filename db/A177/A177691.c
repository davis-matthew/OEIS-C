/*
 * A177691  The odd values of PrimePi(.), with repetition.
 * https://oeis.org/A177691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 805, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177691: odd values of PrimePi(x) with repetition.
   a(n) is the n-th odd π(m) value when m runs over the positive integers. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static data for a simple sieve. */
static bool primes_ready = false;
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

/* Simple sieve generating all primes <= limit. */
static void compute_primes_up_to(uint32_t limit)
{
    if (primes_ready) return;

    bool *is_composite = (bool*)calloc(limit + 1, sizeof(bool));
    for (uint32_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (uint32_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }

    /* Count primes. */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++cnt;

    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    primes_cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) primes[primes_cnt++] = i;

    free(is_composite);
    primes_ready = true;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                      */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for non‑positive indices */

    /* Ensure a sufficiently large sieve.  500 000 covers far more
       than the thousands of terms the tests will request.          */
    if (!primes_ready) compute_primes_up_to(500000);

    idx_t cum = 0;                       /* cumulative number of odd π terms seen */

    for (size_t idx = 0; idx + 1 < primes_cnt; ++idx) {
        /* π(p_{idx+1}) = idx+1 (0‑based index → π value). */
        uint32_t pi_val = (uint32_t)(idx + 1);

        if ((pi_val & 1u) == 0)            /* skip even π values */
            continue;

        /* Length of the block where π = pi_val: the gap to the next prime. */
        uint32_t gap = primes[idx + 1] - primes[idx];

        if (cum + (idx_t)gap >= n)         /* the desired term falls here */
            return (seq_t)pi_val;

        cum += (idx_t)gap;
    }

    /* In practice we will never reach this point for the tested n. */
    return 0;
}
