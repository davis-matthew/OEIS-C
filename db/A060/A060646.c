/*
 * A060646  Bonse sequence: a(n) = minimal j such that n-j+1 < prime(j).
 * https://oeis.org/A060646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = minimal j such that n - j + 1 < prime(j) (OEIS A060646)
 * Range:     u8 n <= 1866, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by a binary search over the prime indices. */
/* A simple sieve precomputes the first 200 000 primes (enough for n ≤ 10 000). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* -------------------------------------------------------------------
 *  Prime sieve
 *  We generate all primes up to LIMIT once and cache them.
 *  LIMIT is large enough for n up to at least 10 000.
 * ------------------------------------------------------------------- */

#define LIMIT 200000          /* about 18 000 primes – more than enough */
#define PRIME_BUF_SIZE 200000 /* enough buffer size */

static uint32_t primes[PRIME_BUF_SIZE];
static size_t   prime_count = 0;
static bool     primes_ready = false;

/* One‑time initialization – builds the sieve and stores primes. */
static void init_primes(void)
{
    if (primes_ready) return;

    static unsigned char composite[LIMIT + 1];
    size_t i, j;
    for (i = 2; i * i <= LIMIT; ++i) {
        if (!composite[i]) {
            for (j = i * i; j <= LIMIT; j += i)
                composite[j] = 1;
        }
    }
    for (i = 2; i <= LIMIT; ++i)
        if (!composite[i])
            primes[prime_count++] = (uint32_t)i;
    primes_ready = true;
}

/* -------------------------------------------------------------------
 *  Utility: return the j‑th prime (1‑indexed)
 * ------------------------------------------------------------------- */
static inline uint32_t prime_at(idx_t j)
{
    /* caller guarantees j ≤ prime_count, j ≥ 1 */
    return primes[j - 1];
}

/* -------------------------------------------------------------------
 *  Main sequence function
 * ------------------------------------------------------------------- */
seq_t a(idx_t n)            /* n ≥ 1 */
{
    init_primes();

    /* The minimal j will never exceed n+1 (for j > n+1 the left side is
       non‑positive and the inequality is automatically satisfied).      */
    idx_t high = (n + 1 < prime_count) ? (n + 1) : prime_count;
    idx_t low  = 1;

    while (low < high) {
        idx_t mid = (low + high) >> 1;

        /* Use signed arithmetic to avoid unsigned wrap‑around */
        int64_t left  = (int64_t)n - (int64_t)mid + 1;
        int64_t right = (int64_t)prime_at(mid);

        if (left < right)
            high = mid;        /* mid satisfies the inequality, try smaller j */
        else
            low = mid + 1;     /* mid fails, go larger */
    }
    return (seq_t)low;
}
