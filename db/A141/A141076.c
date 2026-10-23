/*
 * A141076  Primes of form 2*3*5*7*11 - p, where p is prime.
 * https://oeis.org/A141076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy, less
 * Formula:   a(n) = prime p such that p < 2310 and 2310 - p is also prime
 * Range:     u8 n <= 29, u16 n <= 228, u32 n <= 228, u64 n <= 228,
 *            u128 n <= 228, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *prime_table = NULL;
static size_t prime_count = 0;

/* build the list of p with p < 2310 and 2310-p prime */
static void build_table(void) {
    if (prime_count != 0) return;           /* already built */

    const idx_t target = 2310;              /* 2*3*5*7*11 */
    /* first count how many qualify */
    for (idx_t p = 2; p < target; ++p) {
        if (is_prime(p) && is_prime(target - p)) {
            ++prime_count;
        }
    }

    /* allocate table */
    if (prime_count == 0) return;           /* safety */
    prime_table = (seq_t *)malloc(prime_count * sizeof(seq_t));
    if (!prime_table) return;               /* allocation failed */

    /* fill the table */
    size_t i = 0;
    for (idx_t p = 2; p < target; ++p) {
        if (is_prime(p) && is_prime(target - p)) {
            prime_table[i++] = (seq_t)p;
        }
    }
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset < 1 */
    build_table();
    if (n - 1 < prime_count)              /* in bounds */
        return prime_table[n - 1];
    return 0;                              /* out of bounds */
}
