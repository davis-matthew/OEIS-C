/*
 * A038541  a(n) = number of modules with n elements over the ring of integers
 *          in the imaginary quadratic field of discriminant -20.
 * https://oeis.org/A038541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} f_p(e) where f_p(e) is defined as follows: * p
 *            = 2 or 5 (ramified prime): f_p(e) = partition(e) * (−20|p) = 1
 *            (p splits): f_p(e) = Σ_{i=0}^e partition(i) * partition(e−i) *
 *            (−20|p) = −1 (p inert): if e is odd, f_p(e) = 0 if e is even,
 *            f_p(e) = partition(e/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 9505 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* maximum exponent needed for n ≤ 2^63 (worst case n = 2^63)
   and for inert primes we also need e/2. 64 is ample. */
#define PART_MAX 64

/* partition numbers p(0)…p(PART_MAX) */
static seq_t partition_arr[PART_MAX + 1];
static bool partition_inited = false;

/* compute partition numbers using the pentagonal‑number recurrence */
static void init_partitions(void)
{
    if (partition_inited) return;
    /* p(0) = 1 by definition */
    partition_arr[0] = (seq_t)1;
    for (unsigned i = 1; i <= PART_MAX; ++i)
    {
        long long sum = 0;
        for (unsigned k = 1; ; ++k)
        {
            /* generalized pentagonal numbers */
            long long g1 = (long long)k * (3LL * k - 1) / 2;
            if (g1 > (long long)i) break;
            int sign = (k & 1) ? 1 : -1;      /* (+ + - - + + …) */
            sum += sign * (long long)partition_arr[i - g1];

            long long g2 = (long long)k * (3LL * k + 1) / 2;
            if (g2 > (long long)i) continue;
            sum += sign * (long long)partition_arr[i - g2];
        }
        partition_arr[i] = (seq_t)sum;          /* sum is non‑negative */
    }
    partition_inited = true;
}

/* main function required by the task */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    init_partitions();

    /* factor n */
    /* arrays large enough for the maximum number of distinct prime factors of
       an integer ≤ 2^64 (at most 16). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t idx = 0; idx < cnt; ++idx)
    {
        uint64_t p = primes[idx];
        uint64_t e = exps[idx];
        seq_t f = 0;

        /* ramified primes 2 and 5 */
        if (p == 2 || p == 5)
        {
            f = partition_arr[e];
        }
        else
        {
            /* compute (−20|p) via Jacobi symbol */
            uint64_t a = (p - (20 % p)) % p;       /* (-20) mod p */
            int leg = jacobi_symbol(a, p);

            if (leg == 1)          /* split prime */
            {
                seq_t sum = (seq_t)0;
                for (uint64_t i = 0; i <= e; ++i)
                {
                    sum += partition_arr[i] * partition_arr[e - i];
                }
                f = sum;
            }
            else                    /* inert prime */
            {
                if (e & 1ULL) return (seq_t)0;   /* odd exponent impossible */
                f = partition_arr[e / 2];
            }
        }

        result *= f;
    }

    return result;
}
