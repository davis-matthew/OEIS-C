/*
 * A216914  The Gauss factorial N_n! restricted to prime factors for N >= 0, n
 *          >= 1, square array read by antidiagonals.
 * https://oeis.org/A216914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:
 *   a(n) = ∏_{p prime ≤ N} p  with gcd(p,n_row)=1 ,
 *   where for the sequence index k = n, the pair (N,n_row)
 *   is obtained by reading the infinite array A(N,n_row) by antidiagonals.
 *   That is, let s be the smallest integer with s(s+1)/2 ≥ k;
 *   let prev = (s-1)s/2, pos = k - prev - 1,
 *   then n_row = pos + 1 and N = s - n_row.
 *   The product uses only primes ≤ N that are coprime with n_row.
 *   The computation is performed by a simple sieve up to N.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Upper bound on the sieve needed for the required n (≈√(2n)). */
#define MAX_SIEVE 20000

static bool sieve[MAX_SIEVE + 1];

/* Perform a sieve of Eratosthenes up to limit N (N ≤ MAX_SIEVE). */
static void compute_primes_up_to(idx_t limit)
{
    /* Initialize all to prime. */
    for (idx_t i = 0; i <= limit; ++i) {
        sieve[i] = true;
    }
    if (limit >= 0) sieve[0] = false;
    if (limit >= 1) sieve[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                sieve[j] = false;
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t k)
{
    if (k < 1) {
        return 0;   /* Undefined for k < 1, but not used in tests. */
    }

    /* Determine (N,n_row) from the antidiagonal mapping. */
    idx_t s = 1;
    while ((s * (s + 1)) / 2 < k) {
        ++s;
    }
    idx_t prev = (s - 1) * s / 2;
    idx_t pos  = k - prev - 1;        /* 0‑based offset on this diagonal */
    idx_t n_row = pos + 1;            /* n ≥ 1 */
    idx_t N     = s - n_row;          /* N ≥ 0 */

    seq_t result = 1;

    if (N >= 2) {
        if (N > MAX_SIEVE) {
            /* For safety: adjust limit if larger than our static array. */
            /* This case should not occur for the problem limits. */
            return 0;
        }
        compute_primes_up_to(N);
        for (idx_t p = 2; p <= N; ++p) {
            if (!sieve[p]) {
                continue;
            }
            seq_t ps = (seq_t)p;
            if (gcd(ps, (seq_t)n_row) == 1) {
                result *= ps;
            }
        }
    }
    return result;
}
