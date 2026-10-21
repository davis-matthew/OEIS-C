/*
 * A221491  Number of primes of the form k*n - k + n, 1 <= k <= n.
 * https://oeis.org/A221491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of primes of the form k*n - k + n, 1 <= k <= n
 * Range:     u8 n <= 18, u16 n <= 277, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/*  Sieve bit array: one bit per odd number, but here we keep a bit for
    every integer for simplicity.  The array is expanded on demand by
    the first call to a().  unsigned char is the smallest type that
    can hold a bit mask. */
static idx_t     sieve_limit = 0;
static unsigned char *prime_bits = NULL;

/* Return true if x is marked as prime in the sieve. */
static inline int has_prime_bit(idx_t x)
{
    return (prime_bits[x >> 3] >> (x & 7)) & 1;
}

/* Grow or build a sieve up to at least 'limit' (inclusive). */
static void build_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* If we already have a smaller sieve, free it. */
    if (prime_bits != NULL) {
        free(prime_bits);
        prime_bits = NULL;
    }

    /* Allocate enough bytes to hold 'limit+1' bits. */
    size_t sz = (size_t)(limit >> 3) + 1;      /* cast to size_t for malloc */
    prime_bits = (unsigned char*)malloc(sz);  /* C++ requires cast of void* */
    if (!prime_bits)          /* allocation failure – should not happen */
        return;

    /* Start with all bits set (prime). */
    memset(prime_bits, 0xFF, sz);

    /* 0 and 1 are not prime. */
    prime_bits[0] &= ~((unsigned char)1); /* bit 0 */
    prime_bits[0] &= ~((unsigned char)2); /* bit 1 */

    /* Simple Eratosthenes sieve.  Limit is small enough (≤ n², n≤10 000). */
    for (idx_t p = 2; p <= limit / p; ++p) {
        if (has_prime_bit(p)) {
            for (idx_t m = p * p; m <= limit; m += p) {
                prime_bits[m >> 3] &= ~(1 << (m & 7));
            }
        }
    }

    sieve_limit = limit;
}

/* Sequence a(n).  Valid for all n ≥ 1; a(0) is undefined but handled for safety. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset is 1, but guard for 0 */

    /* We need to know primes up to n² to test each value k*n - k + n. */
    idx_t limit = n * n;
    build_sieve(limit);

    /* Each row is an arithmetic progression:
         f(k) = k*n - k + n = (n-1)*k + n. */
    seq_t start = (seq_t)2 * n - 1;      /* value at k = 1: (n-1)*1 + n = 2n-1 */
    seq_t step  = (seq_t)n - 1;          /* common difference of the row   */
    seq_t val   = start;
    seq_t count = 0;

    /* k runs from 1 to n (inclusive).  We test each value for primality. */
    for (idx_t i = 0; i < n; ++i) {
        if ((idx_t)val >= 2 && has_prime_bit((idx_t)val))
            ++count;
        val += step;
    }
    return count;
}
