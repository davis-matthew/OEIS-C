/*
 * A087556  a(n) = smallest prime that is the concatenation of two numbers
 *          whose sum is n.
 * https://oeis.org/A087556
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   concat, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A087556:  a(n) = smallest prime that can be obtained
   by concatenating two positive integers u and v with u+v = n.
   If no such prime exists (conjecturally for n ≡ 0 (mod 3)), the
   function returns 0.  The routine uses a simple trial‑factor sieve
   up to a fixed limit, marked with *SIEVE_LIMIT*.  The sieve is
   initialized once on the first call.  Even‑number treatment is
   corrected so that composite evens are not reported as primes.
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/concat.h"
#include "../utils/num_digits.h"

/* ----------  prime sieve up to SIEVE_LIMIT  ---------- */
#define SIEVE_LIMIT 50000000U   /* large enough for n ≤ 10000 */
static uint64_t *bitset = NULL;  /* bit array for primality (1 = prime) */
static bool sieve_done = false;

/* helpers for bitset */
static inline bool get_bit(uint32_t x) {
    return (bitset[x >> 6] >> (x & 63U)) & 1ULL;
}
static inline void clear_bit(uint32_t x) {
    bitset[x >> 6] &= ~(1ULL << (x & 63U));
}

/* initialise sieve once */
static void init_sieve(void) {
    if (sieve_done) return;
    size_t bits = (SIEVE_LIMIT >> 6) + 1;
    bitset = (uint64_t*)malloc(bits * sizeof(uint64_t));
    if (!bitset) abort();                     /* insufficient memory */
    memset(bitset, 0xFF, bits * sizeof(uint64_t));  /* assume all prime   */
    clear_bit(0); clear_bit(1);                /* 0 and 1 are not prime */

    /* sieve odd composites */
    for (uint32_t i = 3; (uint64_t)i * i <= SIEVE_LIMIT; i += 2) {
        if (get_bit(i)) {
            for (uint64_t j = (uint64_t)i * i; j <= SIEVE_LIMIT; j += (uint64_t)i << 1)
                clear_bit((uint32_t)j);
        }
    }

    /* mark all even numbers > 2 as composite */
    for (uint32_t i = 4; i <= SIEVE_LIMIT; i += 2)
        clear_bit(i);

    sieve_done = true;
}

/* prime test using the sieve; fallback to trial division for very large n */
static bool is_prime(uint32_t x) {
    if (x <= SIEVE_LIMIT)
        return get_bit(x);
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint32_t p = 3; (uint64_t)p * p <= x; p += 2)
        if (x % p == 0) return false;
    return true;
}

/* ----------  required sequence function  ----------- */
seq_t a(idx_t n)
{
    if (n < 2u) return (seq_t)0;        /* undefined below offset 2 */
    if (n % 3u == 0u) return (seq_t)0;  /* conjectured zeros for multiples of 3 */

    init_sieve();                       /* ensure prime data is ready */

    seq_t best = (seq_t)0;              /* 0 means “not found yet”   */
    for (uint32_t u = 1; u < (uint32_t)n; ++u) {
        uint32_t v = (uint32_t)n - u;
        uint32_t c = concat(u, v);
        if (!is_prime(c)) continue;
        if (best == (seq_t)0 || c < (uint32_t)best) best = (seq_t)c;
    }
    return best;                        /* guaranteed >0 for non‑multiples of 3 */
}
