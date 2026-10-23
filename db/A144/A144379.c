/*
 * A144379  Triangle read by rows, first n terms of an array formed by A054521
 *          * A054521(transform).
 * https://oeis.org/A144379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of integers x (1 ≤ x ≤ k) such that * gcd(x, k) =
 *            1 and gcd(x, r) = 1, * where r is the row containing index n *
 *            (offset_row(r) = r(r-1)/2 + 1) and k = n - offset_row(r) + 1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   _count_coprime_to_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_count_coprime_to_primes.h"

static bool _init_spf = false;
static size_t _spf_limit = 200000; /* sufficient for n up to ~10^4, can be increased */
static uint32_t * _spf_table = NULL;

/* Build smallest prime factor table up to _spf_limit */
static void _init_prime_factors(void)
{
    size_t i, j;
    if (_init_spf) return;
    _spf_table = (uint32_t*)malloc((_spf_limit + 1) * sizeof(uint32_t));
    for (i = 2; i <= _spf_limit; ++i) _spf_table[i] = (uint32_t)i;
    for (i = 2; i * i <= _spf_limit; ++i) {
        if (_spf_table[i] == (uint32_t)i) {
            for (j = i * i; j <= _spf_limit; j += i)
                if (_spf_table[j] == (uint32_t)j) _spf_table[j] = (uint32_t)i;
        }
    }
    _init_spf = true;
}

/* Extract the number of distinct prime factors of m and store them in arr.
 * Returns the count. */
static int _factor_distinct(idx_t m, uint32_t *arr, size_t max_len)
{
    int cnt = 0;
    while (m > 1 && cnt < (int)max_len) {
        uint32_t p = _spf_table[m];
        arr[cnt++] = p;
        while (m % p == 0) m /= p;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but keep for safety */

    /* Ensure prime factor table is built */
    _init_prime_factors();

    /* Find row r such that offset_row(r) <= n < offset_row(r+1) */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;   /* r up to ~sqrt(2n) */

    idx_t offset = r * (r - 1) / 2 + 1;
    idx_t k = n - offset + 1;          /* column index in the triangle */

    /* Gather distinct primes of r and k */
    uint32_t primes[16];   /* max distinct primes for numbers up to 200000 */
    int pcnt = _factor_distinct(r, primes, sizeof(primes)/sizeof(primes[0]));
    idx_t m = k;
    while (m > 1) {
        uint32_t p = _spf_table[m];
        bool exists = false;
        for (int i = 0; i < pcnt; ++i) if (primes[i] == p) { exists = true; break; }
        if (!exists) primes[pcnt++] = p;
        while (m % p == 0) m /= p;
    }

    /* Count numbers <= k coprime to all these primes */
    return _count_coprime_to_primes(k, primes, pcnt);
}
