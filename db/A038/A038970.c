/*
 * A038970  Primes p such that 83 is not a square mod p.
 * https://oeis.org/A038970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th prime p such that Jacobi(83,p) = -1
   (i.e., 83 is not a quadratic residue modulo p).  The first
   term is 2.  The function builds the answer by scanning
   primes and caching the ones that satisfy the Jacobi condition. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/jacobi_symbol.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cached data over successive calls */
    static uint64_t *pvec    = NULL;   /* primes that satisfy the condition */
    static size_t   pcap     = 0;      /* capacity of pvec */
    static size_t   pcnt     = 0;      /* number stored so far   */
    static uint64_t cur      = 3;      /* odd candidate to test   */
    static bool     initialized = false;

    /* First call: initialise cache with the prime 2 */
    if (!initialized) {
        pcap  = 16;
        pvec  = (uint64_t *)malloc(pcap * sizeof(uint64_t));
        pvec[0] = 2ull;   /* 2 is always the first such prime */
        pcnt   = 1;
        cur    = 3ull;    /* start testing from the next odd number */
        initialized = true;
    }

    /* Grow the array when required */
    if (pcnt >= pcap) {
        pcap *= 2;
        pvec = (uint64_t *)realloc(pvec, pcap * sizeof(uint64_t));
    }

    /* Generate primes until we have at least n stored */
    while (pcnt < (size_t)n) {
        /* test primality of the current candidate */
        if (is_prime_big((seq_t)cur)) {
            /* Jacobi(83, cur) returns -1, 0, or 1.
               We want the -1 case. */
            if (jacobi_symbol(83ULL, cur) == -1) {
                pvec[pcnt++] = cur;
                if (pcnt >= pcap) {               /* double if needed */
                    pcap *= 2;
                    pvec = (uint64_t *)realloc(pvec,
                                                pcap * sizeof(uint64_t));
                }
            }
        }
        cur += 2ull; /* skip even numbers */
    }

    return (seq_t)pvec[n - 1];
}
