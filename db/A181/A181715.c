/*
 * A181715  Length of the complete Cunningham chain of the second kind
 *          starting with prime(n).
 * https://oeis.org/A181715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 4239, u32 n <= 5480, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n): length of the Cunningham chain of the second kind starting
 * with the n-th prime.  Compute the number of iterations of 
 * x -> 2*x-1 needed to reach a composite number.
 *
 * Uses a precomputed list of primes sufficient for all n <= 10000.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* pre‑computed primes up to 2,000,000 (∼1.5×10^5 primes)  */
static uint32_t prime_list[200000];
static size_t prime_cnt = 0;
static bool sieve_done   = false;

static void init_primes(void)
{
    if (sieve_done) return;

    const size_t limit = 2000000;
    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool)); /* false = prime */
    for (size_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            prime_list[idx++] = (uint32_t)i;
    free(is_comp);

    prime_cnt = idx;
    sieve_done = true;
}

seq_t a(idx_t n)
{
    init_primes();                 /* ensure primes are ready */

    if (n < 1) return 0;           /* undefined below offset */
    if (n > (idx_t)prime_cnt) return 0;  /* safety check */

    seq_t cur   = (seq_t)prime_list[(size_t)(n - 1)];
    seq_t steps = 0;

    while (true) {
        seq_t nxt = (seq_t)2 * cur - (seq_t)1;
        ++steps;
        if (!is_prime_big(nxt))
            break;
        cur = nxt;
    }
    return steps;
}
