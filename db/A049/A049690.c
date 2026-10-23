/*
 * A049690  a(n) = Sum_{k=1..n} phi(2*k), where phi = Euler totient function,
 *          cf. A000010.
 * https://oeis.org/A049690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} phi(2*k) where phi is Euler's totient
 *            function
 * Range:     u8 n <= 24, u16 n <= 401, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A049690.
 * The implementation builds the totient table up to 2*n on each call,
 * which is fast enough for n up to a few thousand and uses only
 * integer arithmetic with the provided `seq_t` type. */
#include <stdlib.h>  /* for malloc, free */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }

    /* Compute totient values up to 2*n */
    idx_t limit = 2 * n;
    seq_t *phi = (seq_t*)malloc((limit + 1U) * sizeof(seq_t));
    if (!phi) {
        return 0; /* Allocation failed, give up */
    }

    /* Initialize phi[i] = i */
    for (idx_t i = 0; i <= limit; ++i) {
        phi[i] = (seq_t)i;
    }

    /* Sieve to compute phi */
    for (idx_t i = 2; i <= limit; ++i) {
        if (phi[i] == (seq_t)i) {            /* i is prime */
            seq_t ip = (seq_t)i;
            for (idx_t j = i; j <= limit; j += i) {
                /* phi[j] = phi[j] / i * (i - 1) */
                phi[j] = (phi[j] / ip) * (ip - 1U);
            }
        }
    }

    /* Sum phi(2*k) for k = 1..n */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += phi[2 * k];
    }

    free(phi);
    return sum;
}
