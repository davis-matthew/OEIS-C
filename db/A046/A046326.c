/*
 * A046326  Product of 8 successive primes.
 * https://oeis.org/A046326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=0}^{7} p_{n+i} (product of the 8 successive primes)
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 51, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- prime generation ------------------------------------------------ */
static uint64_t *prime_arr = NULL;   /* array holding the primes (1‑based) */
static size_t   prime_cnt = 0;        /* number of primes stored */

/* simple primality test using already found primes */
static inline int is_prime_candidate(uint64_t x)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p * p > x) break;
        if (x % p == 0) return 0;
    }
    return 1;
}

/* ensure we have at least target primes in prime_arr (1‑based index) */
static void ensure_primes(idx_t target)
{
    if (prime_cnt >= (size_t)target) return;

    size_t new_cap = (prime_cnt == 0) ? 8 : prime_cnt * 2;
    while (new_cap < (size_t)target) new_cap *= 2;

    uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
    if (!tmp) { /* allocation failure – give up silently */
        free(prime_arr);
        prime_arr = NULL;
        prime_cnt = 0;
        return;
    }
    prime_arr = tmp;

    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_arr[prime_cnt - 1] + 1;
    while (prime_cnt < (size_t)target) {
        if (is_prime_candidate(candidate))
            prime_arr[prime_cnt++] = candidate;
        ++candidate;
        if (candidate > 2 && candidate % 2 == 0) ++candidate; /* skip even */
    }
}

/* ---------- sequence definition -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n == 0, return 0 */

    /* need primes p_n … p_{n+7} */
    ensure_primes(n + 7);

    if (prime_arr == NULL) return 0;   /* in case of allocation failure */

    seq_t prod = 1;
    size_t start = (size_t)(n - 1);   /* zero‑based offset */
    for (size_t i = 0; i < 8; ++i)
        prod *= (seq_t)prime_arr[start + i];

    return prod;
}
