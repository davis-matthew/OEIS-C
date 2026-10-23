/*
 * A152076  a(n) = the largest prime p < prime(n) such that prime(n) - p is
 *          squarefree, where prime(n) is the n-th prime. a(n) = 0 if no such
 *          prime p exists.
 * https://oeis.org/A152076
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest prime p < prime(n) such that prime(n) – p is
 *            squarefree, or 0 if no such prime exists
 * Range:     u8 n <= 55, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_squarefree.h"

/* Cache of primes up to a fixed upper bound (sufficient for n ≤ 10000). */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t      prime_cnt  = 0;    /* number of primes stored */

/* Initialise the prime list on first use. */
static void init_primes(void)
{
    if (prime_list != NULL) return;
    /* Prime(10000) = 104729. 200000 is comfortably above this. */
    size_t limit = 200000;
    build_primes_upto(limit, &prime_list, &prime_cnt);
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* offset is 2, nothing defined below */

    init_primes();

    if ((size_t)(n - 1) >= prime_cnt)    /* defensive check for very large n */
        return (seq_t)0;

    seq_t prime_n = (seq_t)prime_list[n - 1];

    /* Search downward for the largest prime with squarefree difference. */
    for (idx_t i = n - 1; i > 0; ) {
        --i;                       /* i goes from n-2 down to 0 */
        seq_t diff = prime_n - (seq_t)prime_list[i];
        if (is_squarefree(diff))
            return (seq_t)prime_list[i];
    }

    return (seq_t)0;  /* should never happen for n ≥ 2 */
}
