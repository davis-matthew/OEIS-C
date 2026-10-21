/*
 * A088385  a(n) = prime(n+1)^prime(n+1) - prime(n)^prime(n).
 * https://oeis.org/A088385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n+1)^prime(n+1) - prime(n)^prime(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Dynamic array of primes (1‑based indices). */
static uint32_t *prime_arr = NULL;
static idx_t prime_cnt = 0;
static idx_t prime_cap = 0;

/* Ensure that at least 'needed' primes (1‑based) are available. */
static void ensure_primes(idx_t needed)
{
    if (prime_cnt >= needed)
        return;

    /* Grow the array if necessary. */
    if (prime_cap < needed) {
        prime_cap = needed * 2 + 8;
        prime_arr = (uint32_t *)realloc(prime_arr, prime_cap * sizeof(uint32_t));
        if (!prime_arr) {
            /* Memory exhausted – abort the program as per contest policy. */
            abort();
        }
    }

    /* Start generating primes from the next integer after the last known prime. */
    uint32_t cand = (prime_cnt == 0) ? 2 : prime_arr[prime_cnt - 1] + 1;
    while (prime_cnt < needed) {
        int is_prime = 1;
        for (idx_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_arr[i];
            if ((uint64_t)p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime)
            prime_arr[prime_cnt++] = cand;
        ++cand;
    }
}

/* Fast binary exponentiation: base^exp, with exp as an idx_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1)
            result *= b;
        exp >>= 1;
        if (exp)
            b *= b;
    }
    return result;
}

/* Sequence A088385: a(n) = prime(n+1)^prime(n+1) - prime(n)^prime(n) */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1; return 0 for safety */

    ensure_primes(n + 1);

    uint32_t p_n     = prime_arr[n - 1];   /* prime(n)   */
    uint32_t p_n_plus = prime_arr[n];     /* prime(n+1) */

    seq_t pow_n     = pow_seq((seq_t)p_n, (idx_t)p_n);
    seq_t pow_n_plus = pow_seq((seq_t)p_n_plus, (idx_t)p_n_plus);

    return pow_n_plus - pow_n;
}
