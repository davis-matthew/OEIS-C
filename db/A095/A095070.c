/*
 * A095070  One-bit dominant primes, i.e., primes whose binary expansion
 *          contains more 1's than 0's.
 * https://oeis.org/A095070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = nth prime whose binary expansion contains more 1’s than
 *            0’s
 * Range:     u8 n <= 40, u16 n <= 4235, u32 n <= 8887, u64 n <= 8887,
 *            u128 n <= 8887, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author: brute‑force search with a growing prime list and dominance test. */
/* All computations are on 64‑bit unsigned integers; the results are promoted to seq_t. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Count the number of set bits in a 64‑bit integer. */
static unsigned int popcount_uint64(uint64_t x)
{
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

/* Return the bit‑length of a 64‑bit integer (position of highest set bit + 1). */
static unsigned int bitlen_uint64(uint64_t x)
{
    unsigned int len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* One‑bit dominant prime sequence. */
seq_t a(idx_t n)
{
    /* Static state shared between calls. */
    static bool initialized = false;
    static uint64_t *primes = NULL;      /* list of primes found so far */
    static size_t primes_size = 0, primes_cap = 0;
    static seq_t *dom = NULL;            /* list of one‑bit dominant primes */
    static size_t dom_size = 0, dom_cap = 0;
    static uint64_t candidate = 3;        /* next odd number to test */

    if (!initialized) {
        /* Initialise the prime list with the first prime (2). */
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_size = 1;

        /* Initialise the dominant prime list (empty initially). */
        dom_cap = 16;
        dom = (seq_t *)malloc(dom_cap * sizeof(seq_t));
        dom_size = 0;

        initialized = true;
    }

    /* Generate primes until we have at least n dominant primes. */
    while (dom_size < (size_t)n) {
        uint64_t cand = candidate;
        bool is_prime = true;

        /* Trial division using the primes already found. */
        for (size_t i = 0; i < primes_size; ++i) {
            uint64_t p = primes[i];
            if (p > cand / p)     /* p > sqrt(cand) */
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the new prime if it is not a duplicate. */
            if (primes_size == primes_cap) {
                primes_cap <<= 1;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            if (primes_size == 0 || primes[primes_size - 1] != cand)
                primes[primes_size++] = cand;

            /* Check the dominance condition (more 1's than 0's). */
            unsigned int ones = popcount_uint64(cand);
            unsigned int bits = bitlen_uint64(cand);
            if (ones > bits - ones) {
                if (dom_size == dom_cap) {
                    dom_cap <<= 1;
                    dom = (seq_t *)realloc(dom, dom_cap * sizeof(seq_t));
                }
                dom[dom_size++] = (seq_t)cand;
            }
        }

        /* Move to the next odd number. */
        candidate += 2;
    }

    return dom[n - 1];
}
