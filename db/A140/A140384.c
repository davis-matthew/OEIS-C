/*
 * A140384  Consecutive triples (x=(p^2-1)/2,2*p,y=z=(p^2+1)/2), p prime,
 *          representing isosceles triangles with sides (x,y,z).
 * https://oeis.org/A140384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = * let k = floor((n+2)/3) // index of prime p (starting
 *            from 3) * let p = prime_k * let r = (n-1) % 3 * return: * r==0 :
 *            (p^2 - 1)/2 * r==1 : 2p * r==2 : (p^2 + 1)/2
 * Range:     u8 n <= 15, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static uint64_t *sieve_primes = NULL;   /* primes starting from 3 */
static size_t sieve_count = 0;
static size_t sieve_capacity = 0;

/* Ensure that we have at least 'need' primes in sieve_primes.  All
 * primes are stored in increasing order, starting with 3. */
static void ensure_primes(size_t need) {
    if (sieve_count >= need)
        return;

    /* Grow capacity if necessary.  The allocation is O(need). */
    if (sieve_capacity == 0) {
        sieve_capacity = need;
        sieve_primes = (uint64_t*)malloc(sieve_capacity * sizeof(uint64_t));
    } else if (sieve_capacity < need) {
        sieve_capacity = need;
        sieve_primes = (uint64_t*)realloc(sieve_primes, sieve_capacity * sizeof(uint64_t));
    }

    uint64_t cand = (sieve_count == 0) ? 3 : sieve_primes[sieve_count - 1] + 2;
    while (sieve_count < need) {
        bool is_prime = true;
        for (size_t i = 0; i < sieve_count; ++i) {
            uint64_t p = sieve_primes[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            sieve_primes[sieve_count++] = cand;
        }
        cand += 2;
    }
}

seq_t a(idx_t n) {
    /* 1‑based index of the prime to use */
    idx_t prime_idx = (n + 2) / 3;
    /* position within the current triple */
    idx_t pos = (n - 1) % 3;

    ensure_primes((size_t)prime_idx);
    seq_t p = (seq_t)sieve_primes[prime_idx - 1];
    seq_t p2 = p * p;

    if (pos == 0) {
        /* (p^2 - 1) / 2 */
        return (p2 - (seq_t)1) / (seq_t)2;
    } else if (pos == 1) {
        /* 2p */
        return (seq_t)2 * p;
    } else {
        /* (p^2 + 1) / 2 */
        return (p2 + (seq_t)1) / (seq_t)2;
    }
}
