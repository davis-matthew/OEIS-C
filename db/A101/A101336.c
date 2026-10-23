/*
 * A101336  Alternating addition and subtraction of the residues of the primes
 *          less than the number.
 * https://oeis.org/A101336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, look
 * Formula:   a(n) = Σ_{p<n} (-1)^k · (n mod p), where p are the primes < n in
 *            increasing order (k=0 for the first prime)
 * Range:     i8 n <= 205, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static data for incremental prime generation */
static idx_t _max_sieved   = 1;    /* highest number already processed by the sieve */
static idx_t * _primes     = NULL; /* dynamic array of primes found so far */
static size_t _prime_count = 0;    /* number of primes stored */
static size_t _prime_cap   = 0;    /* allocated capacity */

/* Ensure the primes array can hold at least 'needed' elements */
static void _grow_primes(size_t needed)
{
    if (_prime_cap >= needed) return;
    size_t new_cap = (_prime_cap ? _prime_cap * 2 : 16);
    while (new_cap < needed) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(_primes, new_cap * sizeof(idx_t));
    if (!tmp) abort();            /* out‑of‑memory */
    _primes = tmp;
    _prime_cap = new_cap;
}

/* Extend the sieve to cover all numbers up to 'limit' (inclusive).  All primes < limit+1
   will be stored in the _primes array.  The sieve is incremental: it is only
   performed when a larger limit than any previously processed one is requested. */
static void _sieve_to(idx_t limit)
{
    if (limit <= _max_sieved) return;

    for (idx_t num = _max_sieved + 1; num <= limit; ++num) {
        bool is_prime = true;
        for (size_t i = 0; i < _prime_count; ++i) {
            idx_t p = _primes[i];
            if (p * p > num) break;          /* no need to test larger primes */
            if (num % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            _grow_primes(_prime_count + 1);
            _primes[_prime_count++] = num;
        }
    }
    _max_sieved = limit;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* For n ≤ 2 there are no primes < n, so the alternating sum is 0. */
    if (n <= 2) return 0;

    /* Make sure every prime < n is known. */
    _sieve_to(n - 1);

    seq_t sum = 0;
    for (size_t i = 0; i < _prime_count && _primes[i] < n; ++i) {
        idx_t p   = _primes[i];
        seq_t rem = (seq_t)(n % p);
        if ((i & 1) == 0)    /* even index → plus sign */
            sum += rem;
        else                 /* odd index  → minus sign */
            sum -= rem;
    }
    return sum;
}
