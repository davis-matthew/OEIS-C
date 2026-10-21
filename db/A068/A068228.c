/*
 * A068228  Primes congruent to 1 (mod 12).
 * https://oeis.org/A068228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes ≡ 1 (mod 12).  The function searches candidates
   in increasing order, test them for primality by simple trial
   division, and returns the (n‑1)th found prime.  It works for any
   seq_t type (byte, word, 64‑bit or big integer) and any idx_t
   (wide unsigned indices).  The algorithm is O(n log n) time and
   uses O(n) additional space.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers.
   For numbers ≤ 10⁶ the trivial trial division is fast enough and
   requires only standard arithmetic operations. */
static bool is_prime_uint64(uint64_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Static bookkeeping for incremental generation */
    static uint64_t *primes = NULL;   /* array of found primes (uint64_t) */
    static size_t cnt   = 0;          /* number of primes stored so far */
    static size_t cap   = 0;          /* allocated capacity of the array */
    static uint64_t cand = 13;        /* first candidate ≡ 1 (mod 12) */

    /* One‑time allocation */
    if (primes == NULL) {
        cap = 16;
        primes = (uint64_t*)malloc(cap * sizeof(uint64_t));
    }

    /* Generate primes until we have at least n of them */
    while (cnt < (size_t)n) {
        if (is_prime_uint64(cand)) {
            if (cnt == cap) {           /* grow the array if needed */
                cap <<= 1;
                primes = (uint64_t*)realloc(primes, cap * sizeof(uint64_t));
            }
            primes[cnt++] = cand;        /* store the prime */
        }
        cand += 12;                      /* next candidate ≡ 1 (mod 12) */
    }

    /* Return the n‑th term (1‑based).  The cast to seq_t is safe
       because the prime values themselves fit within 64 bits. */
    return (seq_t)primes[n - 1];
}
