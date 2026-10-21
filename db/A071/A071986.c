/*
 * A071986  Parity of the prime-counting function pi(n).
 * https://oeis.org/A071986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi(n) mod 2 (Parity of the prime‑counting function)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer square‑root for unsigned 64‑bit values.  The result is the floor of sqrt(x). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = ((idx_t)1) << 62;               /* largest power of 4 <= 2^64 */
    while (bit) {
        if (r + bit <= x) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Static storage used to keep the sieve and parity table between calls. */
static idx_t current_max = 0;          /* highest value for which arrays are valid   */
static bool  *is_prime   = NULL;       /* prime flag array (size current_max+1)      */
static seq_t *parity     = NULL;       /* parity of pi(i) for 0 <= i <= current_max */

/* Expand the sieve and parity table to cover all values up to max_n (inclusive). */
static void extend_sieve(idx_t max_n)
{
    if (max_n <= current_max)          /* already sufficient */
        return;

    /* Resize arrays.  A NULL pointer from realloc works like malloc. */
    is_prime = (bool *)realloc(is_prime, (max_n + 1) * sizeof(bool));
    parity   = (seq_t *)realloc(parity,   (max_n + 1) * sizeof(seq_t));

    /* Initialize every entry as a prime candidate. */
    for (idx_t i = 0; i <= max_n; ++i)
        is_prime[i] = true;
    if (max_n >= 0) is_prime[0] = false;
    if (max_n >= 1) is_prime[1] = false;

    /* Sieve of Eratosthenes. */
    idx_t limit = isqrt_u64(max_n);
    for (idx_t p = 2; p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= max_n; q += p)
                is_prime[q] = false;
        }
    }

    /* Compute parity of the prime counter. */
    seq_t count = 0;
    for (idx_t i = 0; i <= max_n; ++i) {
        if (i >= 2 && is_prime[i])
            ++count;
        parity[i] = count & 1U;        /* store 0 or 1 */
    }

    current_max = max_n;
}

/* Public function required by the build.  It must be externally linked. */
seq_t a(idx_t n)
{
    if (n < 1)                 /* outside the offset (undefined), return 0 */
        return (seq_t)0;

    extend_sieve(n);            /* ensure arrays cover n */

    /* The parity array has been correctly filled by extend_sieve. */
    return parity[n];
}
