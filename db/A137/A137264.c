/*
 * A137264  Prime number gaps read modulo 3.
 * https://oeis.org/A137264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (p_{n+1} - p_n) mod 3 where p_k is the k‑th prime (p_1 =
 *            2). a(1) = 1
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


/* A static cache of the first N_PRIMES primes.
   N_PRIMES is chosen large enough for all tests (n <= 10000). */
#define N_PRIMES 20000
static uint64_t primes[N_PRIMES];
static size_t   prime_cnt = 0;

/* Generate prime numbers until we have ‘need’ primes in the cache.
   The algorithm uses incremental trial division; the cache is
   grown only once and then reused for subsequent calls. */
static void generate_primes_up_to(size_t need)
{
    if (prime_cnt >= need)
        return;

    uint64_t cand = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 2;

    while (prime_cnt < need) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes[prime_cnt++] = cand;
        }
        if (cand == 2)
            cand = 3;
        else
            cand += 2;   /* skip even numbers */
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset starts at 1; outside scope returns 0 */
        return (seq_t)0;

    /* We need the (n)th and (n+1)th primes (0‑based indices n-1 and n). */
    generate_primes_up_to((size_t)n + 1);

    uint64_t p_prev = primes[n - 1];
    uint64_t p_next = primes[n];
    uint64_t diff   = p_next - p_prev;

    return (seq_t)(diff % 3);
}
