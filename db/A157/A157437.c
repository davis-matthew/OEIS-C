/*
 * A157437  Primes congruent to 1, 5, 7, or 11 modulo 24.
 * https://oeis.org/A157437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 3254, u32 n <= 9990, u64 n <= 9990,
 *            u128 n <= 9990, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A157437
 * Sequence: primes congruent to 1,5,7 or 11 modulo 24.
 * Offset: 1
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 0 is undefined, return 0 */

    /* static cache of primes in the required residue classes */
    static seq_t *primes = NULL;
    static size_t plen = 0, pcap = 0;
    static idx_t cand = 5;          /* next candidate to test */

    /* ensure cache contains at least n elements */
    while (plen < n) {
        /* test current candidate */
        if (cand % 24 == 1 || cand % 24 == 5 ||
            cand % 24 == 7 || cand % 24 == 11) {
            if (is_prime(cand)) {
                /* grow cache if necessary */
                if (plen == pcap) {
                    size_t newcap = pcap ? pcap << 1 : 8;
                    seq_t *tmp = (seq_t *)realloc(primes, newcap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE); /* allocation failed */
                    primes = tmp;
                    pcap = newcap;
                }
                primes[plen++] = (seq_t) cand;
            }
        }
        cand++;
    }

    return primes[n - 1];
}
