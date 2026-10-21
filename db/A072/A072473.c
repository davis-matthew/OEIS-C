/*
 * A072473  a(n) = prime(2*n) - prime(n).
 * https://oeis.org/A072473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(2*n) - prime(n)
 * Range:     u8 n <= 43, u16 n <= 5749, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Global storage for primes discovered so far. */
static uint64_t *prime_list = NULL;   /* 0‑based index, prime_list[0] == 2 */
static size_t   prime_cap   = 0;
static size_t   prime_cnt   = 0;

/* Ensure that prime_list contains at least 'needed' primes (1‑based). */
static void
ensure_primes(idx_t needed)
{
    if (prime_cnt >= (size_t)needed)
        return;

    /* Grow the array if necessary. */
    if (prime_cap < (size_t)needed) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        if (new_cap < (size_t)needed)
            new_cap = needed;
        void *tmp = realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp)
            abort();                 /* allocation failure – terminate */
        prime_list = (uint64_t *)tmp;
        prime_cap  = new_cap;
    }

    /* Start testing candidates beyond the last known prime. */
    uint64_t last = (prime_cnt == 0) ? 1 : prime_list[prime_cnt - 1];

    while (prime_cnt < (size_t)needed) {
        ++last;                                 /* candidate */
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > last)
                break;
            if (last % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime)
            prime_list[prime_cnt++] = last;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)                 /* offset is 1; behaviour below 1 is undefined */
        return (seq_t)0;

    /* We need the first 2*n primes. */
    ensure_primes(2 * n);

    uint64_t p_n  = prime_list[n - 1];       /* prime(n)   */
    uint64_t p_2n = prime_list[2 * n - 1];   /* prime(2n)  */

    uint64_t diff = p_2n - p_n;
    return (seq_t)diff;                       /* cast to seq_t */
}
