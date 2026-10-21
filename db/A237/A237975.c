/*
 * A237975  Least nonnegative integer m such that for some k = 1, ..., n there
 *          are exactly m^2 twin prime pairs not exceeding k*n.
 * https://oeis.org/A237975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6440, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, is_prime_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A237975
   a(n) = least non‑negative m such that for some k = 1 … n
          the number of twin‑prime pairs ≤ k·n equals m². */

/*  Standard headers  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*  Utility to get integer floor(sqrt(x)) for 64‑bit unsigned values  */
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/is_prime_odd.h"

/*  Main sequence function  */
seq_t a(idx_t n)
{
    /*  For small n the answer is trivially 0 (no twin pairs < 3)  */
    if (n <= 2) return 0;

    /*  Upper bound for all thresholds k·n, 1 ≤ k ≤ n  */
    idx_t limit = n * n;          /* fits in 64‑bit for n up to 10⁴ */

    /*  Allocate a odd‑only sieve: one bit per odd integer 1 … limit  */
    size_t sieveBytes = ((limit >> 1) + 7) >> 3;   /* ceil((limit/2)/8) */
    uint8_t *sieve = (uint8_t *)calloc(sieveBytes, 1);
    if (!sieve) return 0;                    /* recover gracefully */

    /*  Eratosthenes sieve for odd numbers up to sqrt(limit)  */
    idx_t sqrtLimit = _floor_isqrt_u64(limit);
    for (idx_t p = 3; p <= sqrtLimit; p += 2)
    {
        if (!is_prime_odd(p, sieve)) continue;
        idx_t step = p << 1;                 /* 2 * p  */
        for (idx_t j = p * p; j <= limit; j += step)
        {
            idx_t oddIdx = j >> 1;
            sieve[oddIdx >> 3] |= 1U << (oddIdx & 7);
        }
    }

    /*  Scan the interval, keeping a running count of twin pairs  */
    uint64_t twinCnt = 0;
    bool found = false;
    seq_t best = 0;                /* will hold the minimal m   */
    idx_t k = 1;                   /* current threshold multiplier */

    /*  For each odd i such that i+2 ≤ limit  */
    for (idx_t i = 3; i + 2 <= limit; i += 2)
    {
        /*  Thresholds strictly smaller than i+2  */
        while (k <= n && k * n < i + 2)
        {
            idx_t cnt = twinCnt;
            seq_t m = (seq_t)_floor_isqrt_u64(cnt);
            if (m * m == cnt && (!found || m < best))
            {
                best = m;
                found = true;
                if (best == 0) { free(sieve); return 0; }
            }
            ++k;
        }

        /*  Count the twin pair starting at i, if any  */
        if (is_prime_odd(i, sieve) && is_prime_odd(i + 2, sieve))
            ++twinCnt;

        /*  Thresholds equal to i+2  */
        while (k <= n && k * n == i + 2)
        {
            idx_t cnt = twinCnt;
            seq_t m = (seq_t)_floor_isqrt_u64(cnt);
            if (m * m == cnt && (!found || m < best))
            {
                best = m;
                found = true;
                if (best == 0) { free(sieve); return 0; }
            }
            ++k;
        }
    }

    /*  Remaining thresholds that exceed the last processed i+2  */
    while (k <= n && k * n <= limit)
    {
        idx_t cnt = twinCnt;
        seq_t m = (seq_t)_floor_isqrt_u64(cnt);
        if (m * m == cnt && (!found || m < best))
        {
            best = m;
            found = true;
            if (best == 0) { free(sieve); return 0; }
        }
        ++k;
    }

    free(sieve);
    return found ? best : 0;
}
