/*
 * A099243  A variation on Flavius's sieve (A000960): Start with the primes;
 *          at the k-th sieving step, remove every p-th term of the sequence
 *          remaining after the (k-1)-st sieving step, where p is the k-th
 *          prime; iterate.
 * https://oeis.org/A099243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 609, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements the definition of A099243: start with the prime
 * sequence and repeatedly delete every p-th element, where p is the k-th
 * prime.  The remaining elements are the terms of the sequence. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined for n = 0; return 0 as a safe value */
        return (seq_t)0;
    }

    /* Ensure we use 64‑bit arithmetic for index conversions where needed */
    idx_t idx = n;
    /* initial number of primes to consider */
    size_t L = (size_t)idx * 10;
    if (L < 100) L = 100;

    while (1) {
        /* Allocate array big enough to hold L primes */
        seq_t *seq = (seq_t *)malloc(L * sizeof(seq_t));
        if (!seq) return (seq_t)0; /* allocation failure */

        /* Fill with the first L primes */
        for (size_t i = 0; i < L; ++i) {
            seq[i] = (seq_t)get_prime((idx_t)(i + 1));
        }

        size_t cur_sz = L;
        size_t step   = 1; /* step number, corresponds to the k‑th prime */

        /* Perform the deletion steps until the next prime > current size */
        while (1) {
            uint32_t p = get_prime((idx_t)step);
            if ((size_t)p > cur_sz)
                break;

            /* In‑place delete every p‑th element */
            size_t w = 0;
            for (size_t r = 0; r < cur_sz; ++r) {
                if (((r + 1) % p) != 0)
                    seq[w++] = seq[r];
            }
            cur_sz = w;
            ++step;
        }

        /* If we have enough terms, return the requested one */
        if (cur_sz >= (size_t)idx) {
            seq_t result = seq[(size_t)idx - 1];
            free(seq);
            return result;
        }

        /* Not enough terms – enlarge L and retry */
        free(seq);
        L <<= 1; /* double the capacity */
    }
}
