/*
 * A231122  Numbers k >= 0 such that 2^k is number of ways to write n as n =
 *          x*y, where x, y = squarefree numbers, 1 <= x <= n, 1 <= y <= n, or
 *          -1 if no such k exists.
 * https://oeis.org/A231122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the exponent k such that 2^k equals the number of
 * unordered factorizations n = x*y with x and y square‑free.
 * If no such k exists the function returns -1.
 * Algorithm: for each divisor d ≤ √n check that d and n/d are square‑free
 * and count the pairs.  The count must be a power of two; if so k = log₂(count).
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/_floor_isqrt_u64.h"

static uint64_t primes[10001];
static size_t prime_cnt = 0;
static bool primes_ready = false;

/* Build a list of all primes ≤ 10000 (sufficient for n ≤ 10000). */
static void init_primes(void)
{
    if (primes_ready)
        return;
    const size_t limit = 10000;
    bool sieve[limit + 1];
    memset(sieve, true, sizeof(sieve));
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = false;
        }
    }
    for (size_t i = 2; i <= limit; ++i) {
        if (sieve[i])
            primes[prime_cnt++] = i;
    }
    primes_ready = true;
}

/* Return true if n is square‑free. */
static bool is_squarefree_uint64(uint64_t n)
{
    if (n < 2)
        return true;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= n; ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) {
            n /= p;
            if (n % p == 0)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return -1;      /* invalid input */

    init_primes();

    uint64_t count = 0;
    idx_t sqrt_n = _floor_isqrt_u64(n);

    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d != 0)
            continue;
        idx_t e = n / d;
        if (is_squarefree_uint64((uint64_t)d) &&
            is_squarefree_uint64((uint64_t)e))
            ++count;
    }

    if (count == 0)
        return -1;      /* no admissible factorizations */

    if ((count & (count - 1ull)) != 0ull)
        return -1;      /* count is not a power of two */

    /* compute log₂(count) */
    seq_t k = 0;
    uint64_t c = count;
    while (c >>= 1)
        ++k;

    return k;
}
