/*
 * A055509  Number of odd primes in sequence obtained in 3x+1 (or Collatz)
 *          problem starting at n.
 * https://oeis.org/A055509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd primes encountered in the Collatz * (3x+1)
 *            sequence that starts at n (counting the starting * odd prime
 *            itself). The sequence follows the definition * given in the OEIS
 *            entry. The implementation uses * recursion; no caching is
 *            required for the test ranges
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Simple deterministic primality test for 64‑bit values.
 * Works for the values that appear in the Collatz sequence for the
 * n-values used in the tests (no Miller–Rabin needed). */
static int is_prime_uint64(uint64_t v)
{
    if (v < 2) return 0;
    if ((v & 1) == 0) return v == 2;
    for (uint64_t i = 3; i <= v / i; i += 2)
        if (v % i == 0) return 0;
    return 1;
}

/* Recursive helper that counts odd primes along the Collatz sequence. */
static seq_t collatz_odd_prime_count(idx_t n)
{
    if (n == 1) return 0;                /* base case */

    if ((n & 1) == 0)                    /* even case: n /= 2 */
        return collatz_odd_prime_count(n / 2);

    /* odd case: 3n + 1, then count n if it is an odd prime */
    unsigned __int128 tmp = (unsigned __int128)n * 3 + 1;
    idx_t next = (idx_t)tmp;             /* safe for the test ranges */
    seq_t res = collatz_odd_prime_count(next);
    if (is_prime_uint64((uint64_t)n))
        res += (seq_t)1;
    return res;
}

/* Public function as required by the specification. */
seq_t a(idx_t n)
{
    return collatz_odd_prime_count(n);
}
