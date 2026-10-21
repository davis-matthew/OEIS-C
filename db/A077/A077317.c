/*
 * A077317  a(n) is the n-th prime == 1 (mod n).
 * https://oeis.org/A077317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime in the arithmetic progression 1, 1+n,
 *            1+2n, …
 * Range:     u8 n <= 8, u16 n <= 82, u32 n <= 88, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is a direct search; it works for any seq_t that supports
 * basic arithmetic and the is_prime_big() test from the utils directory. */
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Guard against the undefined offset; behaviour for n==0 is irrelevant. */
    if (n == 0) return (seq_t)0;

    seq_t diff = (seq_t)n;         /* common difference n */
    seq_t cand = (seq_t)1 + diff;  /* first term 1 + n */
    seq_t count = 0;

    /* Iterate over the sequence 1 + k*n for k = 1,2,… */
    while (1) {
        if (is_prime_big(cand)) {
            ++count;
            if (count == n)
                return cand;
        }
        cand += diff;
    }
}
