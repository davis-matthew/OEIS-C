/*
 * A071617  a(n) = phi(p)^phi(p) where p = prime(n).
 * https://oeis.org/A071617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = phi(p)^phi(p) where p = prime(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/pow_seq.h"

/* Static state for prime generation */
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;
static idx_t last_checked = 1;

/* Ensure at least n primes are stored in the static array */
static void ensure_nprimes(size_t n)
{
    if (primes_cnt >= n)
        return;

    /* Expand prime array capacity if needed */
    if (primes_cap == 0) {
        primes_cap = 16;
        primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
        if (!primes) exit(EXIT_FAILURE);
    }

    /* Incrementally find next primes until we have enough */
    while (primes_cnt < n) {
        last_checked++;
        bool is_prime = true;
        idx_t root = (idx_t)sqrt((double)last_checked) + 1;
        for (size_t i = 0; i < primes_cnt && primes[i] <= root; ++i) {
            if (last_checked % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
                if (!primes) exit(EXIT_FAILURE);
            }
            primes[primes_cnt++] = last_checked;
        }
    }
}

/* Compute a(n) = (p-1)^(p-1) for the nth prime p */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* Undefined, return 0 */

    ensure_nprimes((size_t)n);
    idx_t p = primes[(size_t)n - 1];
    seq_t base = (seq_t)(p - 1);
    return pow_seq(base, (int)(p - 1));
}
