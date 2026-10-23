/*
 * A126084  a(n) = XOR of first n primes.
 * https://oeis.org/A126084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = XOR of first n primes; a(0) = 0
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static storage for primes and cumulative XORs */
    static uint32_t *primes = NULL;
    static size_t    pcap   = 0;
    static size_t    plen   = 0;
    static uint32_t  candidate = 2;

    static seq_t *xor_arr = NULL;
    static size_t  xcap   = 0;
    static size_t  xlen   = 0;

    /* initialise the XOR array with a(0) = 0 */
    if (xlen == 0) {
        xcap = 16;
        xor_arr = (seq_t *)malloc(xcap * sizeof(seq_t));
        if (!xor_arr) return (seq_t)0;     /* allocation failure returned as 0 */
        xor_arr[0] = (seq_t)0;
        xlen = 1;
    }

    /* generate primes and XORs until we have the nth value */
    while (xlen <= n) {
        /* locate the next prime */
        uint32_t p;
        while (true) {
            bool isprime = true;
            for (size_t i = 0; i < plen; ++i) {
                uint64_t sq = (uint64_t)primes[i] * primes[i];
                if (sq > candidate) break;
                if (candidate % primes[i] == 0) { isprime = false; break; }
            }
            if (isprime) break;
            /* advance to the next odd candidate */
            candidate += (candidate == 2 ? 1 : 2);
        }
        p = candidate;
        /* next candidate after a prime */
        candidate = (p == 2) ? 3 : p + 2;

        /* store the new prime */
        if (plen == pcap) {
            size_t newcap = (pcap == 0) ? 16 : pcap * 2;
            uint32_t *tmp = (uint32_t *)realloc(primes, newcap * sizeof(uint32_t));
            if (!tmp) return (seq_t)0;
            primes = tmp;
            pcap   = newcap;
        }
        primes[plen++] = p;

        /* compute and store the new XOR value */
        if (xlen == xcap) {
            size_t newcap = (xcap == 0) ? 16 : xcap * 2;
            seq_t *tmp = (seq_t *)realloc(xor_arr, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            xor_arr = tmp;
            xcap   = newcap;
        }
        xor_arr[xlen] = xor_arr[xlen - 1] ^ (seq_t)p;
        xlen++;
    }

    return xor_arr[n];
}
