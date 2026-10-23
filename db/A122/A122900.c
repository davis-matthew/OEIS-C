/*
 * A122900  Minimum prime of the form n^k + (n+1)^k for k>1, or 0 if no such
 *          prime exists.
 * https://oeis.org/A122900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn
 * Formula:   a(n) = minimum prime of n^k + (n+1)^k for k>1, otherwise 0
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   is_prime_big, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Convert the index to the sequence type. */
    seq_t nseq  = (seq_t)n;
    seq_t nseq1 = nseq + 1;

    /* Small exponents: 2 .. 64 (inclusive).  We use incremental
       multiplication so each step needs only one big multiplication. */
    seq_t pow_n  = nseq;
    seq_t pow_n1 = nseq1;
    for (idx_t k = 2; k <= 64; ++k) {
        pow_n  = seq_mul(pow_n, nseq);   /* n^k  */
        pow_n1 = seq_mul(pow_n1, nseq1); /* (n+1)^k */
        seq_t sum = pow_n + pow_n1;
        if (is_prime_big(sum))
            return sum;  /* The first prime is the minimal one. */
    }

    /* Larger exponents that are powers of two: 128, 256, 512, 1024. */
    for (idx_t exp = 128; exp <= 1024; exp <<= 1) {
        pow_n  = seq_mul(pow_n,  pow_n);   /* n^exp  */
        pow_n1 = seq_mul(pow_n1, pow_n1);  /* (n+1)^exp */
        seq_t sum = pow_n + pow_n1;
        if (is_prime_big(sum))
            return sum;
    }

    /* No prime found within the tested range. */
    return (seq_t)0;
}
