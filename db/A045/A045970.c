/*
 * A045970  a(1)=7; if n = Product p_i^e_i, n > 1, then a(n) = Product
 *          p_{i+4}^e_i.
 * https://oeis.org/A045970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 7; for n > 1, a(n) = Π p_{i+4}^{e_i}, where n = Π
 *            p_i^{e_i} is the prime factorization of n
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Generate the prime list up to max_limit.
   The list is returned in *out (array of idx_t) and its
   length in *cnt.  The caller is responsible for freeing *out. */
static void build_prime_list(idx_t **out, size_t *cnt, idx_t max_limit)
{
    bool *is_prime = (bool *)malloc((max_limit + 1) * sizeof(bool));
    for (idx_t i = 0; i <= max_limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= max_limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= max_limit; j += i)
                is_prime[j] = false;

    /* count primes */
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= max_limit; ++i)
        if (is_prime[i])
            ++prime_cnt;

    idx_t *primes = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= max_limit; ++i)
        if (is_prime[i])
            primes[pos++] = i;

    free(is_prime);

    *out  = primes;
    *cnt  = prime_cnt;
}

/* Find the index of prime p inside the sorted prime list.
   Assumes p is prime and present in the list. */
static size_t find_prime_index(idx_t p, idx_t *primes, size_t cnt)
{
    size_t low = 0, high = cnt - 1;
    while (low <= high) {
        size_t mid = low + (high - low) / 2;
        if (primes[mid] == p)
            return mid;
        if (primes[mid] < p)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return 0; /* unreachable for valid input */
}

/* Main function requested by the specification. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)7;

    /* Initialize the global prime table once. */
    static bool primes_ready = false;
    static idx_t *prime_list = NULL;
    static size_t prime_count = 0;

    if (!primes_ready) {
        /* 20 000 is comfortably large for n ≤ 10 000 (π(20000)≈2262). */
        build_prime_list(&prime_list, &prime_count, 200000);
        primes_ready = true;
    }

    idx_t tmp = n;
    seq_t result = (seq_t)1;

    /* Trial division using the prime list. */
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= tmp; ++i) {
        idx_t p = prime_list[i];
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            /* Map each prime to the one 4 positions ahead. */
            idx_t mapped = prime_list[i + 4];
            seq_t base  = (seq_t)mapped;
            for (int k = 0; k < exp; ++k)
                result *= base;
        }
    }

    /* Remaining prime factor, if any. */
    if (tmp > 1) {
        size_t idx_remain = find_prime_index(tmp, prime_list, prime_count);
        idx_t mapped = prime_list[idx_remain + 4];
        seq_t base  = (seq_t)mapped;
        result *= base;
    }

    return result;
}
