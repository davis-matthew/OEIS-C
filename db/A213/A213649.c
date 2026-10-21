/*
 * A213649  Smallest k such that there exists a square between prime(n) and
 *          prime(n+k).
 * https://oeis.org/A213649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal positive k such that ∃ integer m with prime(n) <
 *            m^2 < prime(n+k). Indices are 1‑based
 * Range:     u8 n <= 18, u16 n <= 1463, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

/* Prime storage (static to preserve state across calls). */
static bool primes_inited = false;
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
/* Sieve limit – large enough to contain prime(n) and a few following primes
   for n up to ≈10⁴ (prime(10000) ≈ 104 729).  */
#define PRIME_SIEVE_LIMIT 200000

/* Initialise the prime array once. */
static void init_primes(void)
{
    if (primes_inited) return;
    build_primes_upto(PRIME_SIEVE_LIMIT, &primes, &primes_cnt);
    primes_inited = true;
}

/* a(n) implementation */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init_primes();

    /* Adjust for 0‑based array. */
    idx_t idx_start = n - 1;
    if (idx_start >= primes_cnt) return (seq_t)0;   /* safety */

    /* Prime at position n. */
    uint64_t p_start = primes[idx_start];

    /* Largest integer m such that m² ≤ p_start. */
    uint64_t m_floor = (uint64_t)floor_sqrt((seq_t)p_start);
    /* Next square strictly greater than p_start. */
    uint64_t next_square = (m_floor + 1) * (m_floor + 1);

    /* Find first prime after idx_start that exceeds next_square. */
    for (idx_t idx = idx_start + 1; idx < (idx_t)primes_cnt; ++idx) {
        if (primes[idx] > next_square) {
            /* k is the difference of indices: (idx_start+1) distance to idx. */
            return (seq_t)(idx - idx_start);
        }
    }

    /* If we run out of primes (unlikely for the intended ranges), return 0. */
    return (seq_t)0;
}
