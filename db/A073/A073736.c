/*
 * A073736  Sum of primes whose index is congruent to n (mod 3); equals the
 *          partial sums of A073737 (in which sums of three successive terms
 *          form the primes).
 * https://oeis.org/A073736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 258, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Σ{ p_m | 0 ≤ m ≤ n, m ≡ n (mod 3) },
   with the convention p₀ = 1 and p_k the k‑th prime for k ≥ 1. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *primes = NULL;   /* primes[0] = 2 = p₁, primes[i‑1] = p_i for i ≥ 1 */
static size_t   primes_cnt = 0;   /* number of primes stored (should be n + 1) */
static idx_t    max_n = 0;        /* highest n for which primes[0..max_n] are available */

/* Ensure that primes[0 .. n] are available.  */
static void ensure_primes(idx_t n)
{
    if (n <= max_n) return;
    free(primes);
    sieve_first_n_primes(n + 1, &primes, &primes_cnt);
    max_n = n;
}

seq_t a(idx_t n)
{
    ensure_primes(n);

    seq_t sum = 0;
    for (idx_t m = n; ; ) {
        seq_t p;
        if (m == 0)
            p = (seq_t)1;               /* 0th prime is treated as 1 */
        else
            p = (seq_t)primes[m - 1];    /* primes[0] holds p₁ */

        sum += p;

        if (m < 3) break;               /* no lower index under 0 */
        m -= 3;
    }
    return sum;
}
