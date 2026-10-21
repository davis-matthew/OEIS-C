/*
 * A216265  Number of primes between n^3 - n and n^3.
 * https://oeis.org/A216265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p with n³‑n < p ≤ n³ (n ≥ 1)
 * Range:     u8 n <= 5952, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _mr_witness
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_mr_witness.h"

static bool _is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    /* trial division by small primes */
    static const uint64_t small_primes[] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};
    for (idx_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }

    /* write n‑1 as d·2ˢ with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1U;
        ++s;
    }

    /* deterministic bases for 64‑bit numbers */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (idx_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        if (!_mr_witness(n, a, d, s))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset handling – not part of sequence */

    /* bounds of the interval (n³ – n, n³] */
    idx_t high = n * n * n;    /* n³ */
    idx_t low  = high - n;     /* n³ – n */

    seq_t cnt = 0;
    for (idx_t x = low + 1; x <= high; ++x) {
        if (_is_prime_uint64((uint64_t)x))
            ++cnt;
    }
    return cnt;
}
