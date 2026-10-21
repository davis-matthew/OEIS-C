/*
 * A084841  Write the numbers 1, 2, ... in a triangle with n terms in the n-th
 *          row; a(n) = number of deficient integers in n-th row.
 * https://oeis.org/A084841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of deficient integers in rows of the triangle
 *            T(1), T(2), …, T(n); say row n contains the integers T(n−1)+1 ..
 *            T(n). A number is deficient if the sum of its proper divisors is
 *            strictly less than the number itself. This implementation
 *            returns that count by testing each integer in the...
 * Range:     u8 n <= 22, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- prime sieve (executed once) ---------- */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define PRIME_LIMIT 100000

static uint32_t _primes[PRIME_LIMIT];
static size_t _prime_cnt = 0;
static bool _primes_ready = false;

static void _make_primes(void)
{
    static uint8_t is_comp[PRIME_LIMIT + 1] = {0};
    size_t i, j;
    for (i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            for (j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = 1;
    for (i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i])
            _primes[_prime_cnt++] = (uint32_t)i;
    _primes_ready = true;
}

/* --------- sum of divisors (incl. the number itself) ------------ */
static uint64_t _sigma(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t sigma = 1;
    uint64_t m = n;
    size_t k;
    for (k = 0; k < _prime_cnt && (uint64_t)_primes[k] * _primes[k] <= m; ++k)
    {
        uint32_t p = _primes[k];
        if (m % p == 0)
        {
            uint64_t term = 1 + p;
            uint64_t power = p;
            m /= p;
            while (m % p == 0)
            {
                m /= p;
                power *= p;
                term += power;
            }
            sigma *= term;
        }
    }
    if (m > 1)   /* remaining prime factor */
        sigma *= (1 + m);
    return sigma;
}

/* ------------------- sequence entry ----------------------------- */
seq_t a(idx_t n)
{
    if (!_primes_ready)
        _make_primes();

    /* start of nth row: T(n-1)+1 = n*(n-1)/2 + 1 */
    seq_t start = (seq_t)n * ((seq_t)n - 1) / 2 + 1;
    seq_t count_def = 0;
    idx_t i;

    for (i = 0; i < n; ++i)
    {
        seq_t m = start + (seq_t)i;      /* current integer      */
        uint64_t sig = _sigma((uint64_t)m);   /* sum of all divisors  */
        /* deficient iff sigma < 2*m  */
        if (sig < 2 * ((uint64_t)m))
            ++count_def;
    }
    return count_def;
}
