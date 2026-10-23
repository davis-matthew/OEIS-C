/*
 * A163553  First differences of A024816.
 * https://oeis.org/A163553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(n) - sigma(n+1) + n + 1 where sigma(m) is the sum
 *            of all positive divisors of m
 * Range:     i8 n <= 59, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a prime‑factorization routine provided in
   utils by the build.  For each call it computes sigma(n) and
   sigma(n+1) independently.  All arithmetic is performed with the
   seq_t type. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Compute sigma(m) = sum of all positive divisors of m.
   It uses the factor_u64 helper to obtain prime factors.   */
static seq_t sigma_u64(uint64_t m)
{
    seq_t sigma = 0;
    if (m == 0) return 0;          /* not used, but safe */
    if (m == 1) return 1;          /* sigma(1) = 1 */

    uint64_t pr[20], ex[20];       /* 20 is enough for 64‑bit numbers */
    size_t cnt;
    factor_u64(m, pr, ex, &cnt);

    /* Compute product of (p^(e+1)-1)/(p-1) for each prime factor. */
    unsigned long long result = 1;     /* intermediate fits in 64‑bit for m ≤ 1e6 */
    for (size_t i = 0; i < cnt; ++i) {
        unsigned long long p = pr[i];
        unsigned long long e = ex[i];
        unsigned long long p_pow = 1;
        for (unsigned long long k = 0; k < e + 1; ++k)
            p_pow *= p;                /* p^(e+1) */
        unsigned long long term = (p_pow - 1) / (p - 1);
        result *= term;
    }
    sigma = (seq_t)result;
    return sigma;
}

seq_t a(idx_t n)
{
    /* n >= 1, use 64‑bit to avoid overflow when converting to seq_t */
    seq_t sigma_n   = sigma_u64((uint64_t)n);
    seq_t sigma_np1 = sigma_u64((uint64_t)(n + 1));
    seq_t add = (seq_t)(n + 1);

    return sigma_n - sigma_np1 + add;
}
