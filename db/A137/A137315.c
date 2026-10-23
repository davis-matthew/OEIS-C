/*
 * A137315  a(n) is the least number m such that any finite group of order at
 *          least m has at least n automorphisms.
 * https://oeis.org/A137315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 1 + max{ m : phi(m) < n }
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence equals 1 + the largest integer m whose Euler totient phi(m)
   is less than n.  For n = 1.., this gives the OEIS A137315. */

#include <stdint.h>
#include <stdlib.h>

#define LIMIT 2000000              /* sieve limit – ample for n up to ~10 000 */

static uint32_t phi[LIMIT + 1];
static uint32_t best_here[LIMIT + 1];
static uint32_t prefix_max[LIMIT + 1];
static int initialized = 0;

/* ---------- initialization of totients and prefix data ----------
   Compute least prime factor totient sieve up to LIMIT.
   For each phi value, store the largest m that has that phi.
   Then build the prefix maximum array:
   prefix_max[k] = max{ m : phi(m) <= k }   (for k <= LIMIT).
   The desired a(n) then is prefix_max[n-1] + 1, with the convention
   that n-1 == 0 gives 0.  */
static void init(void)
{
    /* totient sieve */
    for (uint64_t i = 0; i <= LIMIT; ++i) phi[i] = (uint32_t)i;
    for (uint64_t p = 2; p <= LIMIT; ++p)
        if (phi[p] == p)            /* p is prime */
            for (uint64_t j = p; j <= LIMIT; j += p)
                phi[j] -= phi[j] / p;

    for (uint64_t i = 0; i <= LIMIT; ++i) best_here[i] = 0;
    for (uint64_t m = 1; m <= LIMIT; ++m) {
        uint32_t ph = phi[m];
        if (ph <= LIMIT && m > best_here[ph])
            best_here[ph] = (uint32_t)m;
    }

    prefix_max[0] = 0;
    for (uint64_t i = 1; i <= LIMIT; ++i) {
        uint32_t v = best_here[i];
        prefix_max[i] = (prefix_max[i-1] > v) ? prefix_max[i-1] : v;
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 0) { /* undefined, but safe */
        return (seq_t)1;
    }

    /* index for lookup: want φ(m) < n, so take n-1. */
    uint64_t ix = (n > 0) ? (n - 1) : 0;
    if (ix > LIMIT) ix = LIMIT;

    uint32_t m = prefix_max[ix];
    return (seq_t)(m + 1);
}
