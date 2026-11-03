/*
 * A191078  Primes p that have Kronecker symbol (p|70) = -1.
 * https://oeis.org/A191078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 994 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate primes p with the Kronecker symbol (p|70) = -1.
   The sequence starts a(1)=3, a(2)=11, a(3)=13, … (OEIS A191078). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Kronecker symbol (p|70) for an odd prime p.
   Return -1 or +1 for p coprime to 70, 0 otherwise. */
static int kronecker70(uint64_t p)
{
    /* Skip primes dividing 70 (symbol is 0). */
    if ((p % 2ULL) == 0ULL || (p % 5ULL) == 0ULL || (p % 7ULL) == 0ULL)
        return 0;

    /* (p|2) */
    int k2 = ((p & 7ULL) == 1ULL || (p & 7ULL) == 7ULL) ? 1 : -1;

    /* (p|5) */
    int r5 = p % 5ULL;
    int k5 = (r5 == 1ULL || r5 == 4ULL) ? 1 : -1;

    /* (p|7) */
    int r7 = p % 7ULL;
    int k7 = (r7 == 1ULL || r7 == 2ULL || r7 == 4ULL) ? 1 : -1;

    return k2 * k5 * k7;
}

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
    seq_t *tmp = mem_resize(seq_t, prime_list, new_cap);
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
