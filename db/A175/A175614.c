/*
 * A175614  a(1)=1, a(2)=5, then a(n)=a(n-2)+4, if n odd else
 *          a(n)=prime(primepi(a(n-2)+4)).
 * https://oeis.org/A175614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=5, a(n)=a(n-2)+4 for odd n, a(n)=prime(2n‑1) for
 *            even n (prime(1)=2)
 * Range:     u8 n <= 27, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *prime_list = NULL;
static size_t prime_list_size = 0;

static void init_prime_list(void)
{
    if (prime_list != NULL)
        return;

    /* Upper bound giving more than 20 000 primes (needed for n up to 10 000). */
    const size_t LIMIT = 400000;
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime)
        return;   /* Allocation failed – the program will return 0 for all even n. */

    for (size_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= LIMIT; q += p)
                is_prime[q] = false;
        }
    }

    /* Count primes to allocate the list. */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            ++prime_list_size;

    prime_list = (seq_t *)malloc(prime_list_size * sizeof(seq_t));
    if (!prime_list) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            prime_list[idx++] = (seq_t)i;

    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    init_prime_list();

    if ((n & 1) == 1) {                /* odd n */
        idx_t k = (n - 1) / 2;
        return ((seq_t)4 * ((seq_t)k)) + (seq_t)1;
    } else {                           /* even n */
        idx_t prime_index = 2 * n - 1;  /* 1‑based index of the prime */
        if (prime_index == 0 || (size_t)prime_index > prime_list_size)
            return (seq_t)0;           /* out of range – should not happen */
        return prime_list[prime_index - 1];
    }
}
