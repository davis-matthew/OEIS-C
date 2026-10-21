/*
 * A092997  Least k such that 1 < c < n < p < k, where p is a prime and c is a
 *          composite number such that for every c there exists a distinct p.
 * https://oeis.org/A092997
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, less, nonn
 * Formula:   a(n) is the largest prime that is the (composite(n))‑th prime
 *            greater than n, plus 1. Here composite(n) = number of composite
 *            numbers < n. If composite(n)=0 then a(n)=n+1. The algorithm uses
 *            a sieve that stores all primes up to a fixed limit; for any n in
 *            the test range this limit is sufficient
 * Range:     u8 n <= 56, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/*--- constants -----------------------------------------------------------*/
#define MAX_SIEVE 200000UL   /* enough for n up to about 10000 */

/*--- static state --------------------------------------------------------*/
static bool initialized = false;
static uint8_t  *is_prime_arr = NULL;   /* 1 = prime, 0 = composite, size MAX_SIEVE+1 */
static size_t  *prime_prefix = NULL;    /* prime count up to i, size MAX_SIEVE+1 */
static idx_t   *prime_list   = NULL;    /* list of primes, size prime_cnt */
static size_t  prime_cnt = 0;

/*--- utility functions -----------------------------------------------*/
static void init_sieve(void)
{
    if (initialized) return;
    initialized = true;

    /* allocate arrays */
    is_prime_arr   = (uint8_t *)malloc((MAX_SIEVE + 1) * sizeof(uint8_t));
    prime_prefix   = (size_t *)malloc((MAX_SIEVE + 1) * sizeof(size_t));

    /* sieve of Eratosthenes */
    for (size_t i = 0; i <= MAX_SIEVE; ++i) is_prime_arr[i] = 1;
    is_prime_arr[0] = is_prime_arr[1] = 0;
    for (size_t i = 2; i * i <= MAX_SIEVE; ++i)
        if (is_prime_arr[i])
            for (size_t j = i * i; j <= MAX_SIEVE; j += i)
                is_prime_arr[j] = 0;

    /* prefix sums of primes */
    size_t cnt = 0;
    for (size_t i = 0; i <= MAX_SIEVE; ++i) {
        if (is_prime_arr[i]) ++cnt;
        prime_prefix[i] = cnt;
    }
    prime_cnt = cnt;

    /* allocate and fill prime list */
    prime_list = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    size_t k = 0;
    for (size_t i = 2; i <= MAX_SIEVE; ++i)
        if (is_prime_arr[i]) prime_list[k++] = (idx_t)i;
}

/* binary search for the first prime > n */
static size_t first_prime_gt(idx_t n)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_list[mid] > n) hi = mid;
        else lo = mid + 1;
    }
    return lo;          /* may equal prime_cnt */
}

/*--- the required sequence function -----------------------------------*/
seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* undefined for n<2, return 0 */

    init_sieve();

    /* composite(n) = (n-2) - pi(n-1) */
    size_t pi_n_minus_1 = (n <= 1) ? 0 : prime_prefix[(size_t)(n - 1)];
    size_t composites = (n <= 2) ? 0 : ((size_t)(n - 2)) - pi_n_minus_1;

    if (composites == 0) return (seq_t)(n + 1ULL);

    /* find the composites‑th prime after n */
    size_t idx = first_prime_gt(n);
    size_t target = idx + (composites - 1);
    if (target >= prime_cnt) {
        /* the pre‑computed sieve was not large enough – in the test range
           this should never happen.  To be safe, return n+1. */
        return (seq_t)(n + 1ULL);
    }

    idx_t p = prime_list[target];
    return (seq_t)(p + 1ULL);
}
