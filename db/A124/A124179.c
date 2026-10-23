/*
 * A124179  a(n) = prime(R(prime(n))) where prime(i) is the i-th prime and
 *          R(p) is the value of the reverse of the digits of p.
 * https://oeis.org/A124179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = prime( reverse_decimal( prime(n) ) )
 * Range:     u8 n <= 6, u16 n <= 168, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/rev_decimal.h"

static seq_t *prime_list = NULL;
static idx_t prime_count = 0;
static bool sieve_inited = false;

/* Generate primes up to a fixed upper bound using a simple sieve.
   The bound is chosen to be large enough to accommodate the
   required indices for n ≤ 10 000.  */
static void init_sieve(void)
{
    if (sieve_inited)
        return;

    /* Upper bound for the sieve (approx 20 000 000).  For
       n up to 10 000 this is sufficient to cover the largest
       index we ever need (≈ 927 401 ⇒ ~15 000 000‑th prime).  */
    const idx_t LIMIT = 20000000U;
    const size_t size_limit = LIMIT + 1;

    unsigned char *is_comp = (unsigned char *)malloc(size_limit);
    if (!is_comp) {
        /* If memory allocation fails, abort.  */
        exit(1);
    }
    memset(is_comp, 0, size_limit);

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = 1;
        }
    }

    /* Count primes to allocate array. */
    idx_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) ++cnt;

    prime_list = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!prime_list) {
        free(is_comp);
        exit(1);
    }

    idx_t idx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            prime_list[idx++] = (seq_t)i;
        }
    }
    prime_count = cnt;

    free(is_comp);
    sieve_inited = true;
}

seq_t a(idx_t n)
{
    /* Arguments are 1‑based.  */
    if (n < 1U)
        return 0;

    init_sieve();

    /* k‑th prime: prime(n) */
    if (n > prime_count)
        return 0;          /* safety – should not happen with chosen limit */

    seq_t p = prime_list[n - 1];

    /* Reverse decimal digits of p.  */
    seq_t rev = rev_decimal(p);

    /* Convert the reversed number to an index.  */
    idx_t rev_idx = (idx_t)rev;
    if (rev_idx < 1U || rev_idx > prime_count)
        return 0;          /* safety – should not happen */

    return prime_list[rev_idx - 1];
}
