/*
 * A130063  Primes p such that p divides 3^((p+1)/2) - 2^((p+1)/2) - 1.
 * https://oeis.org/A130063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1619, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th prime p such that p divides 3^((p+1)/2) - 2^((p+1)/2) - 1
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* static table of found terms */
    static seq_t *table = NULL;
    static idx_t count = 0;    /* number of elements stored in table */
    static uint64_t candidate = 3; /* next odd candidate to test */

    if (n == 0) return 0; /* offset 1; value for n = 0 is unused */

    /* allocate table on first invocation */
    if (table == NULL) {
        table = (seq_t *)malloc(sizeof(seq_t));
        assert(table != NULL);
    }

    /* extend table until it contains n terms */
    while (count < n) {
        /* find the next prime */
        while (!is_prime(candidate))
            candidate += 2;          /* skip even numbers */

        /* compute (p+1)/2 as exponent */
        uint64_t exp = (candidate + 1) >> 1;   /* (p+1)/2 */

        /* compute a3 = 3^exp (mod p) and a2 = 2^exp (mod p) */
        seq_t a3 = mod_pow((seq_t)3, exp, (seq_t)candidate);
        seq_t a2 = mod_pow((seq_t)2, exp, (seq_t)candidate);

        /* check whether p divides 3^exp - 2^exp - 1 */
        seq_t diff = a3;
        diff += (seq_t)candidate; /* make a3 + p to avoid negative */
        diff -= a2;
        diff -= (seq_t)1;
        diff %= (seq_t)candidate;

        if (diff == (seq_t)0) {
            /* store the prime in the table */
            if (count == 0) {
                table[0] = (seq_t)candidate;
            } else {
                seq_t *tmp = (seq_t *)realloc(table, sizeof(seq_t) * (count + 1));
                assert(tmp != NULL);
                table = tmp;
                table[count] = (seq_t)candidate;
            }
            count++;
        }

        candidate += 2;   /* advance to the next odd candidate */
    }

    return table[n - 1];
}
