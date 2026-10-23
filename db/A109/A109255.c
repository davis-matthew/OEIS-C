/*
 * A109255  a(n) = (p^2 - 1) / 12, where p is the n-th prime of the form
 *          4*k+1.
 * https://oeis.org/A109255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p^2 - 1)/12, where p is the nth prime congruent to 1
 *            (mod 4)
 * Range:     u8 none, u16 none, u32 n <= 3721, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Simple trial‑division primality test for the small numbers that
   occur in this sequence (p < 2·10^5 for n ≤ 10000). */
static int is_prime_uint64(uint64_t n)
{
    if (n < 2u)
        return 0;
    if (n == 2u || n == 3u)
        return 1;
    if ((n & 1u) == 0u)
        return 0;

    for (uint64_t i = 3u; i * i <= n; i += 2u)
        if (n % i == 0u)
            return 0;
    return 1;
}

seq_t a(idx_t n)
{
    /* Return 0 for n == 0 (offset is 1, behavior below offset is irrelevant). */
    if (n == 0)
        return 0;

    /* Array large enough to hold all primes of the form 4k+1 up to n = 10000.
       For n = 10000 the 10000‑th such prime < 200 000; 20000 slots
       are far more than sufficient. */
    static uint64_t primes4[20000];
    static idx_t count = 0;          /* number of primes stored so far */
    static uint64_t candidate = 5u;  /* first prime ≡ 1 (mod 4) */

    /* Generate primes of the form 4k+1 until we have at least n of them. */
    while (count < n)
    {
        if (is_prime_uint64(candidate))
            primes4[count++] = candidate;
        candidate += 4u;            /* next integer ≡ 1 (mod 4) */
    }

    uint64_t p = primes4[n - 1];
    seq_t pp = (seq_t)p;            /* cast before arithmetic to stay in seq_t domain */
    seq_t result = (pp * pp + (seq_t)(-1)) / 12;  /* (p^2 – 1)/12 using unsigned arithmetic */
    return result;
}
