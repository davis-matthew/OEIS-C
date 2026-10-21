/*
 * A099910  Number of distinct prime-factors of
 *          ((prime(n)*(prime(n+1)-1)+(prime(n)-1)*prime(n+1))/2).
 * https://oeis.org/A099910
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221(A099909(n)) A099909(n) =
 *            (prime(n)*(prime(n+1)-1)+(prime(n)-1)*prime(n+1))/2 a(n) =
 *            number of distinct prime factors of that value. The offset is 2:
 *            a(2) is the first listed term
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 4809, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   _count_distinct_primes, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"           // uint32_t get_prime(idx_t n);
#include "../utils/_count_distinct_primes.h"  // seq_t _count_distinct_primes(idx_t x);

seq_t a(idx_t n)
{
    /* Return 0 for indices below the sequence offset; behaviour is undefined. */
    if (n < 2)
        return 0;

    /* Fetch the required primes.  get_prime is 1‑based: prime(1)=2. */
    uint32_t p1 = get_prime(n);     /* prime(n)   */
    uint32_t p2 = get_prime(n + 1); /* prime(n+1) */

    /* Convert to seq_t for arithmetic.  seq_t is unsigned. */
    seq_t a1 = (seq_t)p1;
    seq_t a2 = (seq_t)p2;

    /* Compute A099909(n) without overflowing 64‑bit values. */
    seq_t prod = a1 * a2;               /* prime(n) * prime(n+1) */
    seq_t num  = (prod << 1) - a1 - a2; /* 2*prod - a1 - a2   */
    seq_t val  = num >> 1;              /* divide by 2         */

    /* Count distinct prime factors of the result.  The helper accepts idx_t. */
    return _count_distinct_primes((idx_t)val);
}
