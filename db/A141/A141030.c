/*
 * A141030  a(n)=If[Prime[n + 1]^2 - Prime[n]*Prime[n + 2] > 0,
 *          If[Mod[Prime[n]*Sqrt[5], 1] >= 0.5, 0, 1],
 *          If[Mod[Prime[n]*Sqrt[7], 1] >= 0.5, 2, 3]].
 * https://oeis.org/A141030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, less
 * Formula:   a(n) = If[Prime[n + 1]^2 - Prime[n]*Prime[n + 2] > 0,
 *            If[Mod[Prime[n]*Sqrt[5], 1] >= 0.5, 0, 1],
 *            If[Mod[Prime[n]*Sqrt[7], 1] >= 0.5, 2, 3]] Offset: 1 (see
 *            A141030)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *primes = NULL;          /* array of first primes, 0‑based */
static size_t primes_count = 0;          /* number of primes stored */

/* Ensure at least `need` primes are available */
static void ensure_primes(idx_t need)
{
    if (need <= (idx_t)primes_count)
        return;

    if (primes != NULL)
        free(primes);

    /* Rely on the supplied helper to allocate and fill the array */
    sieve_first_n_primes(need, &primes, &primes_count);
}

/* Core sequence function */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined by offset, return 0 for safety */
        return (seq_t)0;

    /* Need p_n, p_{n+1}, p_{n+2} -> ensure n+2 primes */
    ensure_primes(n + 2);

    /* Convert the 1‑based indices to array indices (0‑based) */
    const uint32_t p_n   = primes[(size_t)n - 1];
    const uint32_t p_np1 = primes[(size_t)n];
    const uint32_t p_np2 = primes[(size_t)n + 1];

    /* Compute the sign of p_{n+1}^2 - p_n*p_{n+2} */
    const __int128 diff = (__int128)p_np1 * p_np1
                       - (__int128)p_n   * p_np2;

    /* Constants for fractional‑part comparison (64‑bit precision) */
    const uint64_t SCALE = 1000000000000000000ULL;    /* 10^18 */
    const uint64_t HALF  = SCALE >> 1;               /* 0.5 * SCALE  */
    /* floor(sqrt(5) * SCALE) and floor(sqrt(7) * SCALE) */
    const uint64_t SQRT5 = 2236067977499789696ULL;
    const uint64_t SQRT7 = 2645751311064590591ULL;

    if (diff > 0) { /* use sqrt(5) branch */
        const __int128 prod = (__int128)p_n * SQRT5;
        uint64_t rem = (uint64_t)(prod % SCALE);
        return (rem >= HALF) ? (seq_t)0 : (seq_t)1;
    } else {        /* use sqrt(7) branch */
        const __int128 prod = (__int128)p_n * SQRT7;
        uint64_t rem = (uint64_t)(prod % SCALE);
        return (rem >= HALF) ? (seq_t)2 : (seq_t)3;
    }
}
