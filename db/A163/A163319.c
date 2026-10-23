/*
 * A163319  Primes of the form 3^n-n^3.
 * https://oeis.org/A163319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A163319 – Primes of the form 3^k – k^3 (k = 1, 2, …)
 * This function returns the n‑th such prime, where n ≥ 1.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static cache of found primes, growing as needed */
    static seq_t *primes = NULL;
    static size_t cap = 0, sz = 0;
    static idx_t exp = 0;            /* last exponent processed */

    /* generate primes until we have at least n of them */
    while (sz < n) {
        exp++;                      /* next exponent candidate */

        seq_t power = big_pow((seq_t)3, (seq_t)exp);
        seq_t cube  = (seq_t)exp * (seq_t)exp * (seq_t)exp;
        seq_t val   = power - cube;

        if (is_prime_big(val)) {
            /* ensure capacity and append the prime */
            grow_seq(&primes, &cap, sz + 1);
            primes[sz++] = val;
        }
    }

    return primes[(size_t)(n - 1)];
}
