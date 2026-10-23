/*
 * A119497  Primes of the form k! + k!! + 1.
 * https://oeis.org/A119497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the nth prime of the form k! + k!! + 1 (OEIS A119497).
 * Generates successive values of k! + k!! + 1 and tests them for
 * primality.  Results are memoized so that subsequent calls reuse
 * previously computed data. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_cnt = 0;
static idx_t next_k = 1;
static seq_t fact = (seq_t)1;
static seq_t dodd = (seq_t)1;
static seq_t deven = (seq_t)1;

static void grow_primes(size_t need)
{
    if (need <= primes_cap)
        return;
    size_t new_cap = primes_cap ? primes_cap << 1 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_ptr = (seq_t*)realloc(primes, new_cap * sizeof(seq_t));
    if (!new_ptr)
        exit(1);
    primes = new_ptr;
    primes_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* generate primes until we have at least n of them */
    while (primes_cnt < n) {
        /* factorial update */
        fact = fact * (seq_t)next_k;

        /* double factorial update */
        seq_t df_k;
        if (next_k & 1) {
            df_k = dodd * (seq_t)next_k;
            dodd = df_k;
        } else {
            df_k = deven * (seq_t)next_k;
            deven = df_k;
        }

        /* compute the value k! + k!! + 1 */
        seq_t val = fact + df_k + (seq_t)1;

        if (is_prime_big(val)) {
            grow_primes(primes_cnt + 1);
            primes[primes_cnt++] = val;
        }

        ++next_k;
    }

    return primes[n - 1];
}
