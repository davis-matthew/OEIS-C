/*
 * A108763  If n-th prime is 8m+1, then a(n) = 8m+3. If n-th prime is 8m+3,
 *          then a(n) = 8m+5. If n-th prime is 8m+5, then a(n) = 8m+7. If n-th
 *          prime is 8m+7, then a(n) = 8m+1.
 * https://oeis.org/A108763
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p(n) + 2 if p(n) % 8 != 7 else p(n) - 6, where p(n) is
 *            the n-th prime (prime(1)=2)
 * Range:     u8 n <= 54, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static state: dynamic array holding primes. */
static uint64_t *prime_array = NULL;
static size_t prime_cnt   = 0;  /* number of primes stored */
static size_t prime_cap   = 0;  /* allocated capacity */

/* Ensure that at least `n` primes are stored in prime_array. */
static void ensure_prime_cnt(size_t n)
{
    if (prime_cnt >= n) return;

    /* Initialise array if it hasn't yet been created. */
    if (prime_array == NULL) {
        prime_cap = 128;
        prime_array = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        prime_array[0] = 2;
        prime_array[1] = 3;
        prime_cnt = 2;
    }

    uint64_t candidate = prime_array[prime_cnt - 1] + 2;  /* next odd candidate */

    while (prime_cnt < n) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_array[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = 0; break; }
        }

        if (is_prime) {
            if (prime_cnt >= prime_cap) {
                prime_cap *= 2;
                prime_array = (uint64_t*)realloc(prime_array, prime_cap * sizeof(uint64_t));
            }
            prime_array[prime_cnt++] = candidate;
        }
        candidate += 2; /* stay with odd numbers only */
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;        /* offset is 2 – undefined below. */

    ensure_prime_cnt((size_t)n);

    uint64_t p = prime_array[n - 1];  /* nth prime (prime(1)=2) */

    uint64_t res;
    if ((p & 7ULL) == 7ULL)   /* p % 8 == 7 */
        res = p - 6ULL;
    else
        res = p + 2ULL;

    return (seq_t)res;
}
