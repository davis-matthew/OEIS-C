/*
 * A142731  Primes congruent to 4 mod 59.
 * https://oeis.org/A142731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 4 modulo 59 (OEIS A142731)
 * Range:     u8 n <= 1, u16 n <= 105, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* ---- Prime test ----------------------------------------------------- */
/* Uses the shared utility isPrimeTrial if available; falls back to a
   simple trial division method for small values.  It works for any
   seq_t type that supports +, *, /, % and comparison with small
   constants. */
static bool is_prime_seq(seq_t n)
{
    /* reject non‑positive or even numbers */
    if (n <= (seq_t)1) return false;
    if (n == (seq_t)2) return true;
    if (n % (seq_t)2 == (seq_t)0) return false;

    /* trial division up to sqrt(n) using odd divisors */
    for (seq_t d = (seq_t)3; d * d <= n; d += (seq_t)2)
        if (n % d == (seq_t)0) return false;
    return true;
}

/* ---- Sequence generator --------------------------------------------- */
/* Static cache of found primes.  The size 200000 exceeds the number of
   terms needed for n up to at least 10⁴ (the 10 000-th such prime is
   < 6×10⁶).  The array is large enough to avoid dynamic allocation
   which would complicate compatibility with both C and C++.          */
static seq_t cache[200000];
static size_t cache_size = 0;      /* number of filled entries    */
static idx_t  next_k     = 0;      /* 4 + 59*next_k candidate    */

/* Public function required by the framework */
seq_t a(idx_t n)
{
    /* Return immediately if the cache already contains the value */
    if (n <= cache_size)
        return cache[n - 1];

    /* Generate candidates until we have at least n primes */
    while (cache_size < (size_t)n)
    {
        seq_t candidate = (seq_t)4 + (seq_t)59 * (seq_t)next_k;
        ++next_k;

        if (is_prime_seq(candidate))
        {
            /* Store the new prime */
            cache[cache_size++] = candidate;
        }
    }

    return cache[n - 1];
}
