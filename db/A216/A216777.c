/*
 * A216777  Primes p such that x^64 = -2 has no solution mod p.
 * https://oeis.org/A216777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p for which the congruence x^64 = –2 has no solution modulo p.
   A prime p is included iff (-2)^((p-1)/gcd(64,p-1)) ≠ 1 (mod p). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Static storage for the computed sequence elements */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;

    /* Last integer that was checked for primality */
    static uint64_t last_checked = 1;

    /* Validate n */
    if (n == 0)
        return (seq_t)0; /* Undefined for n=0, but return 0 */

    /* Extend the sequence until we have at least n terms */
    while (seq_len < n)
    {
        last_checked++;
        /* Skip 2 – it is a prime but does not satisfy the condition */
        if (last_checked == 2)
            continue;

        /* Skip even numbers > 2 */
        if ((last_checked & 1ULL) == 0)
            continue;

        /* Test for primality */
        if (!is_prime(last_checked))
            continue;

        /* Compute gcd(64, p-1).  Since 64 = 2^6 we only need the 2‑adic
           valuation of p−1. */
        uint64_t p = last_checked;
        uint64_t x = p - 1;
        int v2 = 0;
        while ((x & 1ULL) == 0)
        {
            v2++;
            x >>= 1;
        }
        int shift = v2 < 6 ? v2 : 6;
        uint64_t g = 1ULL << shift;
        uint64_t exp = (p - 1) / g;

        /* Compute (-2)^exp mod p.  In modulo p arithmetic -2 ≡ p-2. */
        seq_t res = mod_pow((seq_t)(p - 2), exp, (seq_t)p);

        /* If the result is not 1, p is a term of the sequence. */
        if (res != (seq_t)1)
        {
            grow_seq(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = (seq_t)p;
        }
    }

    return seq[n - 1];
}
