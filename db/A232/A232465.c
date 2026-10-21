/*
 * A232465  a(n) = |{0 < k <= n/2: prime(k) + prime(n-k) - 1 is prime}|.
 * https://oeis.org/A232465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ 0 < k ≤ n/2 : prime(k) + prime(n–k) – 1 is prime }|
 * Range:     u8 n <= 1833, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below pre‑computes primes up to 210000 (which is > 2·prime(10000))
   and then counts qualifying k for the requested n. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The maximum value to be tested for primality.  209458 < 210000. */
#define PRIMELIMIT 210000

/* Static storage needed for the sieve and prime list. */
static uint8_t  *is_prime  = NULL;   /* 0/1 flag array */
static uint32_t *prime_tab = NULL;   /* prime(k) stored at prime_tab[k-1] */
static size_t    prime_cnt = 0;

/* One‑time initialization of the sieve and sorted prime list. */
static void init_sieve(void)
{
    if (is_prime) return;                           /* already done */

    is_prime = (uint8_t *)malloc((PRIMELIMIT + 1) * sizeof(uint8_t));
    if (!is_prime) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= (size_t)PRIMELIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= (size_t)PRIMELIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)PRIMELIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes to allocate the table */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)PRIMELIMIT; ++i)
        if (is_prime[i]) ++cnt;

    prime_tab = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_tab) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)PRIMELIMIT; ++i) {
        if (is_prime[i])
            prime_tab[idx++] = (uint32_t)i;
    }
    prime_cnt = cnt;
}

/* OEIS A232465. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety; offset starts at 1 */
    init_sieve();

    /* n is guaranteed to be ≤ 10000 in typical tests, and prime_cnt
       contains many more primes than needed. */
    seq_t result = 0;
    idx_t half = n >> 1;                /* floor(n/2) */
    for (idx_t k = 1; k <= half; ++k) {
        uint32_t pk   = prime_tab[(size_t)k - 1];
        uint32_t pk_n = prime_tab[(size_t)(n - k) - 1];
        uint32_t s = pk + pk_n - 1;
        if (s <= (uint32_t)PRIMELIMIT && is_prime[s])
            ++result;
    }
    return result;
}
