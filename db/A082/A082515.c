/*
 * A082515  a(n)=A000720(n)+A000010(n).
 * https://oeis.org/A082515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000720(n) + A000010(n) where A000720(n) is π(n) (number
 *            of primes ≤ n) and A000010(n) is φ(n) (Euler's totient function)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for prime sieve */
static idx_t *prime_list = NULL;    /* array of primes */
static size_t prime_count = 0;      /* number of primes stored */
static size_t prime_cap   = 0;      /* current capacity of prime_list */
static idx_t  prime_limit = 0;      /* largest number that has been sieved */

/* ensure that primes up to limit are generated */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* expand capacity if needed */
    if (prime_limit == 0) {
        prime_cap = 32;
        prime_list = (idx_t *)malloc(prime_cap * sizeof(idx_t));
    }

    /* skip 1; start from the larger of 2 and prime_limit+1 */
    idx_t start = prime_limit + 1;
    if (start < 2)
        start = 2;

    for (idx_t num = start; num <= limit; ++num) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_list[i];
            if (p * p > num)
                break;
            if (num % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 32;
                prime_list = (idx_t *)realloc(prime_list,
                                              prime_cap * sizeof(idx_t));
            }
            prime_list[prime_count++] = num;
        }
    }
    prime_limit = limit;
}

/* binary search: number of primes ≤ n */
static idx_t prime_pi(idx_t n)
{
    if (n < 2)
        return 0;

    size_t lo = 0, hi = prime_count - 1;
    idx_t res = 0;
    while (lo <= hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] <= n) {
            res = mid + 1;
            lo = mid + 1;
        } else {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return (idx_t)res;
}

/* Euler's totient φ(n) using the already sieved primes */
static seq_t phi(idx_t n)
{
    seq_t result = (seq_t)n;
    idx_t temp = n;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p * p > temp)   /* p > sqrt(temp) */
            break;
        if (temp % p == 0) {
            result -= result / (seq_t)p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)           /* remaining prime factor > sqrt(n) */
        result -= result / (seq_t)temp;
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0; safe fallback */

    ensure_primes_up_to(n);

    seq_t phi_val = phi(n);
    seq_t pi_val  = (seq_t)prime_pi(n);

    return phi_val + pi_val;
}
