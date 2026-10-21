/*
 * A077157  a(0) = 1, followed by next n composite numbers followed by next n
 *          primes.
 * https://oeis.org/A077157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A077157 – permutation formed by alternating blocks of
    k composites then k primes, starting with a(0)=1.
    The function generates the sequence on the fly using a simple
    primality test.  It works for any  idx_t ≥ 0 and any
    seq_t that can hold natural numbers.  */
#include <stdbool.h>
#include <stdint.h>

/* simple deterministic primality test, good for the values needed
   (≤ a few 10⁶ for n up to ~10⁴).  Uses 128‑bit arithmetic to avoid
   overflow when squaring i. */
static inline bool is_prime(idx_t n)
{
    if (n < 2) return false;
    if ((n & 1) == 0) return n == 2;
    for (idx_t i = 3; (__uint128_t)i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

/* return the next unused composite number > *cur .
   Advances *cur to that value. */
static inline seq_t next_composite(idx_t *cur)
{
    do {
        ++(*cur);
    } while (*cur <= 1 || is_prime(*cur));
    return (seq_t)(*cur);
}

/* return the next unused prime number > *cur .
   Advances *cur to that value. */
static inline seq_t next_prime(idx_t *cur)
{
    do {
        ++(*cur);
    } while (!is_prime(*cur));
    return (seq_t)(*cur);
}

/* sequence term a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    idx_t lastComposite = 1;     /* last composite used (≤ 1 so first next is 4) */
    idx_t lastPrime     = 1;     /* last prime used (≤ 1 so first next is 2) */
    idx_t index         = 1;     /* position of the next term to generate */
    idx_t blockSize     = 1;     /* current block size */

    while (1) {
        /* output blockSize composites */
        for (idx_t i = 0; i < blockSize; ++i) {
            seq_t v = next_composite(&lastComposite);
            if (index == n) return v;
            ++index;
        }

        /* output blockSize primes */
        for (idx_t i = 0; i < blockSize; ++i) {
            seq_t v = next_prime(&lastPrime);
            if (index == n) return v;
            ++index;
        }

        ++blockSize;             /* increase block size for next round */
    }
}
