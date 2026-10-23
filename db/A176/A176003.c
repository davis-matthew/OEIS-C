/*
 * A176003  a(n) = 3*(n-th noncomposite number) - 2.
 * https://oeis.org/A176003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*A008578(n) - 2
 * Range:     u8 n <= 24, u16 n <= 2451, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence element is the nth noncomposite number (1,2,3,5,7,11,13,17,…). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static idx_t *prime_list = NULL;
static size_t prime_used = 0;   /* number of primes currently stored */
static size_t prime_cap  = 0;   /* capacity of the array */

/* Ensure that prime_list contains at least 'need' primes. */
static void _ensure_primes(size_t need)
{
    if (prime_used >= need)
        return;

    /* grow the capacity if necessary */
    if (prime_cap < need) {
        size_t new_cap = need * 2;
        idx_t *tmp = (idx_t*)realloc(prime_list, new_cap * sizeof(idx_t));
        if (!tmp)   /* allocation failure – abort gracefully */
            return;
        prime_list = tmp;
        prime_cap = new_cap;
    }

    /* search for the next primes starting after the last stored one */
    idx_t candidate = prime_used ? prime_list[prime_used - 1] + 1 : 2;

    while (prime_used < need) {
        if (is_prime(candidate)) {
            prime_list[prime_used++] = candidate;
        }
        candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;   /* A008578(1) = 1 → 3*1-2 = 1 */

    /* the n‑th noncomposite is the (n-2)th prime (0‑indexed). */
    _ensure_primes(n - 1);     /* need at least (n-1) primes */
    seq_t p = (seq_t)prime_list[n - 2];
    return p * 3 - 2;
}
