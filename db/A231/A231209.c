/*
 * A231209  Smallest squarefree number k with 2^n ways to write k as k = x*y,
 *          where x, y = squarefree numbers, 1 <= x <= y <= k.
 * https://oeis.org/A231209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 1 (n=0) or the product of the first (n+1) primes (n>=1).
 *            This is the smallest squarefree number with 2^n unordered
 *            squarefree factorizations
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-03)
 * Helpers:   is_prime_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_i32.h"
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t *prime_prod = NULL;   /* prime_prod[r] = product of first r primes, r>=0 */
static size_t prod_len = 0;        /* number of products stored */
static size_t prod_cap = 0;        /* allocation capacity for prime_prod */

static uint64_t *prime_list = NULL;/* list of primes found so far */
static size_t prime_len = 0;       /* number of primes found */
static size_t prime_cap = 0;       /* allocation capacity for prime_list */
static uint64_t next_candidate = 2;/* next integer to test for primality */

static void ensure_primorial_size(idx_t r_needed)
/* grow prime_prod[] and prime_list[] until prime_prod[r_needed] is computed */
{
    if (prime_prod == NULL) {
        /* initialise storage for prime_prod[0] = 1 */
        prod_cap = 16;
        prime_prod = mem_new(seq_t, prod_cap);
        prime_prod[0] = (seq_t)1;
        prod_len = 1;

        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_len = 0;
    }

    while (prod_len <= (size_t)r_needed) {

        /* find next prime */
        while (!is_prime_i32(next_candidate))
            next_candidate++;

        /* store prime */
        if (prime_len == prime_cap) {
            prime_cap <<= 1;
            prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        }
        prime_list[prime_len++] = next_candidate;

        /* compute new product: prime_prod[prod_len] = prime_prod[prod_len-1] * next_candidate */
        seq_t new_prod = mul_small(prime_prod[prod_len-1], (unsigned int)next_candidate);

        if (prod_len == prod_cap) {
            prod_cap <<= 1;
            prime_prod = mem_resize(seq_t, prime_prod, prod_cap);
        }
        prime_prod[prod_len++] = new_prod;

        /* advance candidate for subsequent searches */
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t r_needed = n + 1;                 /* r = number of primes to multiply */
    ensure_primorial_size(r_needed);        /* guarantees prime_prod[r_needed] exists */

    return prime_prod[(size_t)r_needed];
}
