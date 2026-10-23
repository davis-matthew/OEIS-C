/*
 * A118469  Triangle read by rows: a(n,m) = If(n = 1, then 1, else Prime(n) -
 *          1 + Sum_{k=n..m} (Prime(k + 1) - Prime(k))/2 ).
 * https://oeis.org/A118469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if column = 1, otherwise a(n) = (Prime(column) +
 *            Prime(row+1)) / 2 - 1 where the linear index n is decomposed
 *            into row (r) and column (c) by reading the triangle row‑wise.
 *            For row r, the first index is 1 + (r-1)*r/2
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of primes (1‑indexed) */
    static uint64_t *primes = NULL;      /* primes[1] = 2, primes[2] = 3, ... */
    static size_t prime_count = 0;       /* number of cached primes */
    static size_t prime_capacity = 0;    /* allocated size (excluding index 0) */

    if (n == 0) /* safety, although n>=1 in all calls */
        return (seq_t)0;

    /* 1. Convert linear index to row (r) and column (c). */
    uint64_t idx = n - 1;   /* 0‑based index */
    uint64_t r = 1;
    /* Find smallest r such that r*(r+1)/2 > idx. */
    while ((r * (r + 1)) / 2 <= idx)
        ++r;
    uint64_t prev_tri = (r - 1) * r / 2;
    uint64_t c = idx - prev_tri + 1;   /* 1‑based column */

    /* 2. If column == 1, the value is always 1. */
    if (c == 1)
        return (seq_t)1;

    /* 3. Ensure we have primes up to index r+1 (needed for P(r+1)). */
    size_t target = (size_t)(r + 1);   /* primes up to this many entries */
    if (prime_count < target) {
        /* Allocate or reallocate cache */
        if (prime_capacity < target) {
            prime_capacity = target;
            uint64_t *newpr = (uint64_t *)realloc(primes,
                                                 sizeof(uint64_t) * (prime_capacity + 1));
            primes = newpr;
        }

        /* Find next primes until we reach 'target' count. */
        uint64_t cand = (prime_count == 0) ? 2 : primes[prime_count] + 1;
        while (prime_count < target) {
            /* simple primality test using cached primes up to sqrt(cand) */
            bool is_prime = true;
            for (size_t i = 1; i <= prime_count; ++i) {
                uint64_t p = primes[i];
                if (p * p > cand)
                    break;
                if (cand % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                ++prime_count;
                primes[prime_count] = cand;
            }
            ++cand;
        }
    }

    /* 4. Compute the triangle entry using the simplified formula. */
    seq_t p_col   = (seq_t)primes[c];       /* Prime(c)   */
    seq_t p_next  = (seq_t)primes[r + 1];   /* Prime(r+1) */
    seq_t sum     = p_col + p_next;
    seq_t val     = sum / 2 - 1;
    return val;
}
