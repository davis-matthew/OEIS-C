/*
 * A217858  Odd part of lcm(1,2,3,...,n).
 * https://oeis.org/A217858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm of all odd integers k with 1 ≤ k ≤ n (i.e., the odd
 *            part of lcm(1,2,⋯,n))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;          /* undefined for n < 1, but harmless */
    }

    /* Allocate a boolean array to mark composite numbers up to n. */
    idx_t limit = n;
    size_t sz = (size_t)(limit + 1);
    bool *is_composite = (bool *)calloc(sz, sizeof(bool));
    if (!is_composite) {
        return (seq_t)0;          /* allocation failure unlikely for n ≤ 10000 */
    }

    /* Simple Sieve of Eratosthenes. */
    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_composite[j] = true;
            }
        }
    }

    seq_t result = (seq_t)1;
    seq_t nseq   = (seq_t)n;

    /* For each odd prime p, multiply the highest power of p that does not
       exceed n.  This yields the odd part of lcm(1..n). */
    for (idx_t p = 3; p <= limit; p += 2) {
        if (!is_composite[p]) {
            seq_t pow = (seq_t)p;
            while ((pow * (seq_t)p) <= nseq) {
                pow *= (seq_t)p;
            }
            result *= pow;
        }
    }

    free(is_composite);
    return result;
}
