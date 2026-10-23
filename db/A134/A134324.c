/*
 * A134324  Number of Eisenstein-Jacobi primes whose modulus is > n and <=
 *          n+1.
 * https://oeis.org/A134324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=n^2+1}^{(n+1)^2} A055667(k)
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of Eisenstein‑Jacobi primes with modulus > n and <= n+1 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>      /* for sqrt((double)n) */

/* Local helpers to manipulate a packed bit array */
static inline bool bit_test(const uint8_t *bits, idx_t n)
{
    return (bits[n >> 3] >> (n & 7)) & 1;
}
static inline void bit_set(uint8_t *bits, idx_t n)
{
    bits[n >> 3] |= (1U << (n & 7));
}
static inline void bit_clear(uint8_t *bits, idx_t n)
{
    bits[n >> 3] &= ~(1U << (n & 7));
}

/* Prime sieve up to 'limit', stored in a static bit array */
static uint8_t *prime_bits = NULL;   /* packed bits, prime_bits[i]==1 iff i is prime */
static idx_t  prime_bits_len = 0;    /* number of bits actually needed (max n+1) */

/* Ensure prime_bits contains the primality of all numbers 0..limit */
static void ensure_primes(idx_t limit)
{
    if (prime_bits_len > limit) return;   /* already sufficient */

    size_t new_len = (size_t)limit + 1;           /* number of bits */
    size_t bytes   = (new_len + 7) >> 3;         /* bytes needed */

    uint8_t *new_bits = (uint8_t *)malloc(bytes);
    if (!new_bits) return;                      /* allocation failed – the build will fail anyway */

    /* Initially assume everything is prime */
    memset(new_bits, 0xFF, bytes);
    if (new_len > 0)  bit_clear(new_bits, 0);
    if (new_len > 1)  bit_clear(new_bits, 1);

    /* Sieve */
    idx_t sqrt_limit = (idx_t)sqrt((double)limit);
    for (idx_t i = 2; i <= sqrt_limit; ++i) {
        if (!bit_test(new_bits, i)) continue;
        for (idx_t j = i * i; j <= limit; j += i) {
            bit_clear(new_bits, j);
        }
    }

    /* Replace old array */
    free(prime_bits);
    prime_bits       = new_bits;
    prime_bits_len   = new_len;
}

/* Count Eisenstein‑Jacobi prime associates of norm k.  Returns 0, 6, or 12. */
static seq_t count_eisenstein_norm(idx_t k)
{
    if (k == 3u)          return (seq_t)6;        /* ramified prime 3 */

    /* Prime k with k ≡ 1 (mod 3) → two conjugate primes → 12 associates */
    if (k >= 2u && bit_test(prime_bits, k) && (k % 3u) == 1u) {
        return (seq_t)12;
    }

    /* k is a perfect square of a prime ≡ 2 (mod 3) → one prime of norm k → 6 associates */
    if (k > 0) {
        idx_t r = (idx_t)sqrt((double)k);
        if ((r + 1u) * (r + 1u) <= k) ++r;
        while (r * r > k) --r;
        if (r * r == k && bit_test(prime_bits, r) && (r % 3u) == 2u) {
            return (seq_t)6;
        }
    }
    return (seq_t)0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine the range of k values */
    idx_t low  = n * n + 1u;
    idx_t high = (n + 1u) * (n + 1u);

    /* Make sure primes up to 'high' are known */
    ensure_primes(high);

    seq_t sum = 0;
    for (idx_t k = low; k <= high; ++k) {
        sum += count_eisenstein_norm(k);
    }
    return sum;
}
