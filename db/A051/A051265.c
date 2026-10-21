/*
 * A051265  Maximal value of prime divisors of numbers in reduced residue
 *          system for n.
 * https://oeis.org/A051265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max_{1 ≤ k ≤ n , gcd(k,n)=1} ω(k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ω(k) = number of distinct prime factors of k */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd64_uint64.h"

/* dynamically computed array of distinct prime‑factor counts */
static seq_t *omega = NULL;          /* omega[i] = ω(i) */
static size_t omega_size = 0;

/* recompute omega[0..limit] if necessary */
static void ensure_omega(size_t limit)
{
    if (omega == NULL || limit + 1 > omega_size) {
        seq_t *new_omega = (seq_t *)realloc(omega, (limit + 1) * sizeof(seq_t));
        if (!new_omega) return;   /* out‑of‑memory: simply return, caller will yield 0 */
        omega = new_omega;
        omega_size = limit + 1;

        /* initialize */
        for (size_t i = 0; i <= limit; ++i)
            omega[i] = 0;

        /* simple sieve to count distinct prime factors */
        for (size_t p = 2; p <= limit; ++p) {
            if (omega[p] == 0) {          /* p is prime */
                for (size_t m = p; m <= limit; m += p)
                    omega[m]++;           /* increment distinct prime count */
            }
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* safety for non‑offset calls */
    uint64_t N = (uint64_t)n;
    ensure_omega(N);

    seq_t max_omega = 0;
    for (uint64_t k = 1; k <= N; ++k) {
        if (gcd64_uint64(k, N) == 1) {
            seq_t w = omega[(size_t)k];
            if (w > max_omega) max_omega = w;
        }
    }
    return max_omega;
}
