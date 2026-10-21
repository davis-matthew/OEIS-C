/*
 * A061009  a(n) = -2 + Sum_{j=1..n} (-(n-1)!) mod n.
 * https://oeis.org/A061009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 93, i16 n <= 93, i32 n <= 93, i64 n <= 93, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A061009
    Formula:  a(n) = -2 + Σ_{j=1..n} (-(j-1)!) mod j
    Initial values: a(1) = -2,  a(2) = -1,  a(3) = 0.
    For n ≥ 4 this equals π(n), the number of primes ≤ n.
*/

#include <stdlib.h>
#include <stdint.h>

/*---------  Sieve state  -------------------------------------------*/
static uint8_t *prime_flags = NULL;      /* 1 = prime, 0 = composite */
static idx_t  *prime_cum   = NULL;      /* primes≤i for all i */
static idx_t   sieve_limit = 0;          /* largest n that has been sieved */

/*---------  Grow the sieve up to at least n  ------------------------*/
static void grow_sieve(idx_t n)
{
    if (n <= sieve_limit) return;      /* already sufficient */

    /* Allocate new storage */
    uint8_t *new_flags = (uint8_t *)malloc(((size_t)n + 1) * sizeof(uint8_t));
    idx_t   *new_cum   = (idx_t *)malloc(((size_t)n + 1) * sizeof(idx_t));
    if (!new_flags || !new_cum) {
        abort();                       /* allocation failed */
    }

    /* Initialise all as prime, then unmark 0 and 1 */
    for (idx_t i = 0; i <= n; ++i) new_flags[i] = 1;
    new_flags[0] = 0;
    if (n >= 1) new_flags[1] = 0;

    /* Classic sieve of Eratosthenes */
    for (idx_t i = 2; i <= n / i; ++i) {
        if (new_flags[i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                new_flags[j] = 0;
            }
        }
    }

    /* Build cumulative prime count array */
    idx_t cum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        if (new_flags[i]) ++cum;
        new_cum[i] = cum;
    }

    /* Replace the old tables */
    free(prime_flags);
    free(prime_cum);
    prime_flags = new_flags;
    prime_cum   = new_cum;
    sieve_limit = n;
}

/*---------  Main sequence function  ---------------------------------*/
seq_t a(idx_t n)
{
    /* Explicitly handle the first three terms */
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)-1;
    if (n == 3) return (seq_t)0;

    /* For n ≥ 4, a(n) = π(n) */
    grow_sieve(n);
    return (seq_t)prime_cum[n];
}
