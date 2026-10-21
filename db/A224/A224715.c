/*
 * A224715  The number of unordered partitions {a,b} of prime(n) such that a
 *          or b is a nonnegative composite and the other is prime.
 * https://oeis.org/A224715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 257, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A224715
 *  The number of unordered partitions {a,b} of prime(n) such that
 *  one part is a nonnegative composite and the other part is prime.
 *  Computation is performed via a sieve of Eratosthenes and a lookup
 *  of the n‑th prime from get_prime.h.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"    /* get_prime(idx_t) */

static unsigned char *prime_flag = NULL;   /* sieve flags */
static size_t       sieve_n   = 0;          /* size of current sieve */

/*  Ensure that prime_flag contains a sieve up to at least 'limit'.
 *  The sieve is rebuilt when a larger limit is required. */
static void ensure_sieve_upto(idx_t limit)
{
    if (prime_flag == NULL || (size_t)limit >= sieve_n) {
        size_t new_n = (size_t)limit + 1;
        unsigned char *new_flag = (unsigned char *)malloc(new_n);
        if (new_flag == NULL) exit(1);

        for (size_t i = 0; i < new_n; ++i) new_flag[i] = 1;
        if (new_n > 0) new_flag[0] = 0;
        if (new_n > 1) new_flag[1] = 0;

        size_t maxp = new_n - 1;
        for (size_t p = 2; p * p <= maxp; ++p) {
            if (new_flag[p]) {
                for (size_t m = p * p; m <= maxp; m += p)
                    new_flag[m] = 0;
            }
        }

        free(prime_flag);
        prime_flag = new_flag;
        sieve_n   = new_n;
    }
}

/*  a(idx_t n)   –   OEIS sequence A224715 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint32_t p = get_prime(n);           /* p = prime(n) */
    ensure_sieve_upto(p);

    seq_t cnt = 0;
    for (uint32_t a_val = 1; a_val <= p / 2; ++a_val) {
        uint32_t b_val = p - a_val;

        bool a_prime = prime_flag[a_val];
        bool b_prime = prime_flag[b_val];

        bool a_comp = (a_val > 1) && !a_prime;   /* composite (>=4) */
        bool b_comp = (b_val > 1) && !b_prime;

        if ((a_comp && b_prime) || (b_comp && a_prime))
            ++cnt;
    }
    return cnt;
}
