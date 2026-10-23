/*
 * A018450  Divisors of 429.
 * https://oeis.org/A018450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   sequence of positive divisors of 429 in increasing order (finite
 *            sequence)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Comparer for qsort. Works for all seq_t types. */
static int compare_seq(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* Static cache for the eight divisors. */
static seq_t divisor_cache[8];
static bool divisor_cache_ready = false;
static size_t divisor_count = 0;

/* Compute the divisors of 429 once and store them sorted. */
static void ensure_divisors(void)
{
    if (divisor_cache_ready)
        return;

    /* Factor 429 = 3 * 11 * 13. The following simple trial division works for any
       small constant, but is general enough for the purpose of this sequence. */
    const uint64_t target = 429ULL;
    uint64_t primes_f[] = {2, 3, 5, 7, 11, 13, 17, 19};
    uint64_t exps_f[8];
    size_t nf = 0;
    uint64_t n = target;
    for (size_t i = 0; i < 8 && primes_f[i] * primes_f[i] <= n; ++i)
    {
        if (n % primes_f[i] == 0)
        {
            uint64_t e = 0;
            do { n /= primes_f[i]; ++e; } while (n % primes_f[i] == 0);
            primes_f[nf] = primes_f[i];
            exps_f[nf] = e;
            ++nf;
        }
    }
    if (n > 1)
    {
        primes_f[nf] = n;
        exps_f[nf] = 1;
        ++nf;
    }

    /* Generate all divisors using a simple multiplicative algorithm. */
    seq_t temp[8];
    size_t count = 1;
    temp[0] = 1;
    for (size_t f = 0; f < nf; ++f)
    {
        uint64_t p = primes_f[f];
        uint64_t e = exps_f[f];
        seq_t pp = 1;
        for (uint64_t k = 1; k <= e; ++k)
        {
            pp = pp * p;
            size_t old = count;
            for (size_t i = 0; i < old; ++i)
            {
                temp[count++] = temp[i] * pp;
            }
        }
    }

    qsort(temp, count, sizeof(seq_t), compare_seq);
    for (size_t i = 0; i < count; ++i)
        divisor_cache[i] = temp[i];
    divisor_count = count;
    divisor_cache_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0 || n < 1)
        return 0;
    ensure_divisors();
    if ((idx_t)n <= (idx_t)divisor_count)
        return divisor_cache[n - 1];
    return 0;
}
