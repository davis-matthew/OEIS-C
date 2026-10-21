/*
 * A077546  Primes of the form floor(n*Pi).
 * https://oeis.org/A077546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 2124, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077546:  primes of the form ⌊n·π⌋ (n ≥ 1) */
/*   For each n, a(n) is the nth prime that can be written as ⌊k·π⌋ for some integer k. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* cache of found primes */
static seq_t *prime_vals = NULL;
static size_t  prime_cnt  = 0;
static size_t  prime_cap  = 0;
static idx_t   next_k     = 0;   /* next k to test */

/* Ensure at least n primes of the form ⌊k·π⌋ are stored */
static void ensure_primes(idx_t n)
{
    /* Allocate or grow the array if required */
    if (prime_cnt == prime_cap)
    {
        prime_cap = prime_cap ? prime_cap << 1U : 16U;
        prime_vals = (seq_t *)realloc(prime_vals, prime_cap * sizeof(seq_t));
    }

    /* Precise integer representation of π: ⌊π·10¹⁸⌋ and the scale */
    const __int128 PI   = (__int128)3141592653589793238ULL;   /* ⌊π·10¹⁸⌋ */
    const __int128 SCALE= (__int128)1000000000000000000ULL;   /* 10¹⁸ */

    /* Generate candidates until we have enough primes */
    while (prime_cnt < n)
    {
        ++next_k;                                 /* next k */

        /* compute ⌊k·π⌋ exactly */
        __int128 prod = PI * (__int128)next_k;
        seq_t val = (seq_t)(prod / SCALE);

        if (val < 2) continue;                    /* ignore values < 2 */

        /* test primality (works for 64‑bit values) */
        if (is_prime((uint64_t)val))
        {
            if (prime_cnt == prime_cap)            /* grow if needed */
            {
                prime_cap <<= 1U;
                prime_vals = (seq_t *)realloc(prime_vals, prime_cap * sizeof(seq_t));
            }
            prime_vals[prime_cnt++] = val;
        }
    }
}

/* Sequence term a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */
    ensure_primes(n);
    return prime_vals[n - 1];
}
