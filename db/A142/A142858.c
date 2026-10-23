/*
 * A142858  Primes congruent to 60 mod 61.
 * https://oeis.org/A142858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that p ≡ 60 (mod 61)
 * Range:     u8 none, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static state to allow calls in arbitrary order */
    static seq_t *primes = NULL;             /* stored sequence values   */
    static size_t plen  = 0;                 /* number of values stored  */
    static size_t pcap  = 0;                 /* allocated capacity       */
    static uint64_t cand_k = 0;              /* next k to test in 61*k+60 */
    static bool initialized = false;

    if (!initialized) {
        pcap = 128;                           /* initial capacity   */
        primes = (seq_t *)malloc(pcap * sizeof(seq_t));
        initialized = true;
    }

    /* generate more terms if needed */
    while (plen < (size_t)n) {
        uint64_t p64;
        seq_t p;
        /* find next prime congruent to 60 mod 61 */
        while (true) {
            p64 = 61ULL * cand_k + 60ULL;      /* candidate */
            p = (seq_t)p64;
            if (is_prime((idx_t)p))
                break;                        /* prime found   */
            cand_k++;
        }
        /* store the prime */
        if (plen == pcap) {                    /* grow array if full */
            pcap *= 2;
            primes = (seq_t *)realloc(primes, pcap * sizeof(seq_t));
        }
        primes[plen++] = p;
        cand_k++;                              /* advance to next candidate  */
    }

    return primes[(size_t)n - 1];
}
