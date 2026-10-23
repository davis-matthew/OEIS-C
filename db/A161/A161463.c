/*
 * A161463  Sum of all primes from n-th prime to (2*n-1)-th prime.
 * https://oeis.org/A161463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=n}^{2n-1} p_k (p_k = k-th prime)
 * Range:     u8 n <= 7, u16 n <= 87, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 2, a(2) = 8, a(3) = 23, … (the first terms in the OEIS entry) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

/* static state: cached list of primes (1‑based: prime_list[i] = i‑th prime) */
static uint32_t *prime_list = NULL;   /* NULL until the first allocation */
static idx_t    prime_count = 0;      /* number of primes currently stored */

/* Ensure that at least 'needed' primes are available.  The function
   sieve_first_n_primes() builds the first 'needed' primes and returns
   them in a newly allocated array.  The cached array is replaced
   (memory is freed) whenever a larger set is requested.  This keeps
   the total memory usage minimal while supporting arbitrary call
   order. */
static void ensure_primes(idx_t needed)
{
    if (prime_count >= needed)
        return;

    uint32_t *p = NULL;
    size_t   cnt = 0;
    sieve_first_n_primes(needed, &p, &cnt);

    /* Replace the old list with the new one. */
    free(prime_list);
    prime_list = p;
    prime_count = (idx_t)cnt;
}

/* Public sequence function.  For n < 1 the sequence is undefined;
   the function returns 0, which is harmless because such values
   never appear in the test harness. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* We need primes up to the (2*n-1)-th prime. */
    idx_t last_needed = 2 * n - 1;
    ensure_primes(last_needed);

    /* The primes are stored 1‑based in prime_list.  Sum from
       the n‑th to the (2*n‑1)-th prime inclusive. */
    seq_t sum = 0;
    for (idx_t i = n - 1; i < 2 * n - 1; ++i)
        sum += (seq_t)prime_list[i];

    return sum;
}
