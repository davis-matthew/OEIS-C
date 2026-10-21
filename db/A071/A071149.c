/*
 * A071149  Numbers n such that the sum of the first n odd primes (A071148) is
 *          prime; analogous to A013916.
 * https://oeis.org/A071149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer n such that the sum of the first n odd
 *            primes is prime (implemented by incremental prime generation and
 *            primality testing)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 17, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Static state to allow non‑increasing calls */
    static seq_t *qual = NULL;
    static idx_t qlen = 0;
    static size_t qcap = 0;
    static uint64_t cand = 3;          /* current odd candidate to test for primality */
    static idx_t primecnt = 0;         /* count of odd primes seen so far (n) */
    static seq_t sum = 0;              /* running sum of odd primes */

    while (qlen < n) {
        /* Find the next odd prime */
        while (!is_prime_small((seq_t)cand))
            cand += 2;

        /* Update state with this prime */
        primecnt++;
        sum += (seq_t)cand;

        /* If the sum is prime, record the current n */
        if (is_prime_big(sum)) {
            if (qlen == qcap) {
                size_t newcap = qcap ? qcap * 2 : 16;
                seq_t *newp = (seq_t *)malloc(newcap * sizeof(seq_t));
                if (!newp) exit(1);
                if (qual) {
                    memcpy(newp, qual, qlen * sizeof(seq_t));
                    free(qual);
                }
                qual = newp;
                qcap = newcap;
            }
            qual[qlen++] = (seq_t)primecnt;
        }

        /* Next odd candidate */
        cand += 2;
    }

    return qual[n - 1];
}
