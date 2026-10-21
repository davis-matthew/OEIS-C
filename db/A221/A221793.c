/*
 * A221793  Partial sums of cuban primes A002407, that is, primes equal to the
 *          difference of two consecutive cubes.
 * https://oeis.org/A221793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of the first n cuban primes (cuban primes are primes
 *            of the form 3k^2-3k+1)
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 97, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq_array.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0)
        return 0;

    /* Static state */
    static seq_t *cuban_sums = NULL;   /* array of cumulative sums */
    static size_t sums_cap = 0;        /* current capacity of the array */
    static size_t sums_len = 0;        /* number of terms already computed */
    static idx_t next_k = 2;           /* next integer k to test (k starts at 2) */

    /* Extend the cache until we have n terms */
    while (sums_len < n) {
        /* Find the next cuban prime */
        while (1) {
            seq_t kseq = (seq_t)next_k;
            seq_t f = 3 * kseq * kseq - 3 * kseq + 1;  /* 3k^2-3k+1 */

            if (is_prime_big(f)) {
                /* Compute cumulative sum */
                seq_t sum = (sums_len == 0) ? f : cuban_sums[sums_len - 1] + f;

                /* Grow array if necessary */
                grow_seq_array(&cuban_sums, &sums_cap, sums_len + 1);

                /* Store the new sum */
                cuban_sums[sums_len] = sum;
                sums_len++;

                /* Advance k for the next search */
                next_k++;
                break;
            }
            next_k++;
        }
    }

    return cuban_sums[n - 1];
}
