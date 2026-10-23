/*
 * A031913  a(n) = prime(9*n - 6).
 * https://oeis.org/A031913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(9*n - 6) (index offset 1)
 * Range:     u8 n <= 6, u16 n <= 727, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static seq_t *prime_list   = NULL; /* array of computed primes (1‑based) */
static idx_t  prime_count  = 0;    /* number of primes currently stored */

/* Ensure that prime_list contains at least 'need' primes.
   The list is expanded using next_prime, which only needs the
   previous prime as a start value.  Memory is reallocated if
   necessary. */
static void ensure_primes(idx_t need)
{
    if (prime_count >= need)
        return;

    /* Expand the array if needed */
    if (prime_list == NULL) {
        prime_list = (seq_t *)malloc(need * sizeof(seq_t));
        if (!prime_list) abort();
    } else if (prime_count < need) {
        seq_t *tmp = (seq_t *)realloc(prime_list, need * sizeof(seq_t));
        if (!tmp) abort();
        prime_list = tmp;
    }

    /* Generate missing primes */
    while (prime_count < need) {
        uint64_t next;
        if (prime_count == 0)
            next = 2;
        else
            next = next_prime(((uint64_t)prime_list[prime_count - 1]));
        prime_list[prime_count] = (seq_t)next;
        ++prime_count;
    }
}

seq_t a(idx_t n)
{
    /* a(1) is defined; behavior for n < 1 is undefined but we guard. */
    if (n == 0) return 0;

    /* Index of the desired prime (1‑based) */
    idx_t k = 9 * n - 6;
    if (k == 0) return 0; /* unreachable for n >= 1 */

    ensure_primes(k);
    return prime_list[k - 1];
}
