/*
 * A178153  Difference between the numbers of quadratic residues (mod p) less
 *          than p/2 and greater than p/2, where p=prime(n).
 * https://oeis.org/A178153
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..(p-1)/2} (j|p), where p = prime(n) Legendre
 *            symbol (j|p) = 1 for quadratic residues, -1 for nonresidues. The
 *            sum equals 2*#residues - (p-1)/2
 * Range:     u8 n <= 2066, u16 n <= 2066, u32 n <= 2066, u64 n <= 2066,
 *            u128 n <= 2066, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

static seq_t *cache = NULL;      /* memoized sequence values */
static bool   *computed = NULL;  /* flags for cache entries */
static size_t  cache_size = 0;   /* number of allocated slots */

static unsigned char *qr_buffer = NULL;  /* temporary QR lookup buffer */
static size_t qr_buf_size = 0;           /* allocated size of qr_buffer  */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* offset is 2; values below are irrelevant */

    /* Ensure cache can hold index n */
    size_t new_size = (size_t)(n + 1);
    if (new_size > cache_size) {
        seq_t  *new_cache  = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        bool   *new_comp   = (bool *)realloc(computed, new_size * sizeof(bool));
        if (!new_cache || !new_comp) {
            /* Allocation failure – return a safe value */
            return 0;
        }
        /* Zero the newly allocated part */
        memset(new_cache  + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
        memset(new_comp   + cache_size, 0, (new_size - cache_size) * sizeof(bool));
        cache     = new_cache;
        computed  = new_comp;
        cache_size = new_size;
    }

    if (computed[(size_t)n])
        return cache[(size_t)n];

    /* Obtain the n-th prime (1‑based) */
    uint32_t p = (uint32_t)get_prime(n);

    /* For primes ≡ 1 (mod 4) the sum is zero */
    if (p % 4 == 1) {
        cache[(size_t)n] = 0;
        computed[(size_t)n] = true;
        return 0;
    }

    /* Ensure the QR buffer is large enough */
    size_t required = (size_t)p;
    if (required > qr_buf_size) {
        unsigned char *tmp = (unsigned char *)realloc(qr_buffer, required);
        if (!tmp) {
            cache[(size_t)n] = 0;
            computed[(size_t)n] = true;
            return 0;
        }
        qr_buffer = tmp;
        qr_buf_size = required;
    }

    /* Clear buffer */
    memset(qr_buffer, 0, qr_buf_size);

    /* Mark quadratic residues modulo p */
    for (uint32_t i = 1; i < p; ++i) {
        uint32_t r = (uint32_t)(((uint64_t)i * i) % p);
        qr_buffer[r] = 1;
    }

    /* Count residues among 1 .. (p-1)/2 */
    uint32_t half  = (p - 1) / 2;
    uint32_t count = 0;
    for (uint32_t j = 1; j <= half; ++j)
        if (qr_buffer[j])
            ++count;

    /* a(n) = 2*count - half */
    seq_t result = (seq_t)(2 * count - half);
    cache[(size_t)n]    = result;
    computed[(size_t)n] = true;
    return result;
}
