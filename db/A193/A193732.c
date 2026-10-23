/*
 * A193732  Connell-like sequence.
 * https://oeis.org/A193732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A193732: a(n) is formed by blocks of odd/even integers whose
   lengths are successive prime numbers.  The first block consists of odd
   numbers, the next of even numbers, and the parity alternates thereafter.
   Let p_k be the k-th prime.  Block k contains p_k consecutive integers
   of parity (odd if k is odd, even if k is even), starting with the
   smallest integer of that parity not yet used. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* primality test on the list of primes already found */
static bool is_prime_candidate(uint64_t x, uint64_t *primes, size_t cnt)
{
    if (x < 2) return false;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static storage for primes */
    static uint64_t primes[200000];
    static size_t primes_cnt = 0;       /* number of primes stored so far */
    static uint64_t next_candidate = 2;/* integer to test for primality */

    idx_t cumulative = 0;   /* number of terms produced so far */
    seq_t start = 1;        /* first integer of the current block */

    size_t block_idx = 0;   /* index of the block (0‑based) */

    for (;;)
    {
        /* ensure we have at least block_idx+1 primes */
        while (block_idx >= primes_cnt)
        {
            /* find next prime */
            while (!is_prime_candidate(next_candidate, primes, primes_cnt))
                ++next_candidate;
            primes[primes_cnt++] = next_candidate;
            ++next_candidate;          /* advance to the next integer */
        }

        uint64_t p = primes[block_idx];
        idx_t len = (idx_t)p;          /* length of this block */

        /* if the desired position lies within this block, compute it */
        if (cumulative + len >= n)
        {
            idx_t pos = n - cumulative - 1; /* 0‑based index within block */
            return start + (seq_t)(2 * pos);
        }

        /* move start to the first integer of the opposite parity after this block */
        start += (seq_t)(2 * len - 1);
        cumulative += len;
        ++block_idx;
    }
}
