/*
 * A191078  Primes p that have Kronecker symbol (p|70) = -1.
 * https://oeis.org/A191078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, kronecker70
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate primes p with the Kronecker symbol (p|70) = -1.
   The sequence starts a(1)=3, a(2)=11, a(3)=13, … (OEIS A191078). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */
#include "../utils/kronecker70.h"

/* Internal storage for qualifying primes */
static seq_t *prime_list   = NULL;
static size_t prime_count  = 0;
static size_t prime_cap    = 0;
static uint64_t last_checked = 1;   /* last integer examined */

/* Ensure prime_list has capacity for at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (need <= prime_cap)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 32;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Sequence -- returns the n-th prime with (p|70) = -1 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 0 */

    while (prime_count < (size_t)n) {
        /* Find next integer that is a prime > 2 */
        do {
            ++last_checked;
        } while (last_checked == 2ULL || !is_prime((idx_t)last_checked));

        if (kronecker70(last_checked) == -1) {
            ensure_capacity(prime_count + 1);
            prime_list[prime_count++] = (seq_t)last_checked;
        }
    }

    return prime_list[n - 1];  /* 1‑based indexing */
}
