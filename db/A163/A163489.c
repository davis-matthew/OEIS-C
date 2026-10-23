/*
 * A163489  Indices n such that composite(n)/3 is prime.
 * https://oeis.org/A163489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = index of the nth composite number such that
 *            composite(index)/3 is prime
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime.h"

#define MAX_TERMS 500000   /* should comfortably hold a(n) for n up to at least 10000 */

seq_t a(idx_t n)
{
    static seq_t seq[MAX_TERMS];
    static size_t seq_len = 0;     /* number of values already stored */
    static idx_t last_num = 3;     /* next integer to test (starts at 4) */
    static idx_t comp_rank = 0;    /* number of composites seen so far */

    /* Generate terms until the requested index is available */
    while (seq_len < (size_t)n) {
        ++last_num;
        if (is_prime(last_num))          /* skip primes */
            continue;
        ++comp_rank;                    /* increment composite index */

        if ((last_num % 3) == 0) {
            idx_t q = last_num / 3;
            if (is_prime(q))            /* q must be prime */
                seq[seq_len++] = comp_rank;
        }
    }

    return seq[n - 1];
}
