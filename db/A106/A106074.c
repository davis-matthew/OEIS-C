/*
 * A106074  Primes p such that 3*p + 22 and 22*p + 3 are primes.
 * https://oeis.org/A106074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   return the n-th prime p such that 3*p+22 and 22*p+3 are also
 *            prime
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function caches already-found terms, so calls with arbitrary n are fast. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

static bool isPrime64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    const int nb = sizeof(bases)/sizeof(bases[0]);

    for (int i = 0; i < nb; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = 1, p = d, base = a;
        /* modular exponentiation */
        while (p) {
            if (p & 1) x = (unsigned __int128)x * base % n;
            base = (unsigned __int128)base * base % n;
            p >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % n;
            if (x == n-1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* cached sequence data */
static seq_t *seq = NULL;
static idx_t seq_size = 0;
static idx_t seq_cap = 0;
static uint64_t next_candidate = 5;

/* external function requested by OEIS */
seq_t a(idx_t n) {
    /* initialise cache */
    if (!seq) {
        seq_cap = 128;
        seq = seq_malloc(seq_cap);
        seq_size = 0;
    }

    /* generate terms until we have at least n of them */
    while (seq_size < n) {
        /* check current candidate */
        if (isPrime64(next_candidate) &&
            isPrime64(3 * next_candidate + 22) &&
            isPrime64(22 * next_candidate + 3)) {

            if (seq_size == seq_cap) {
                seq_cap <<= 1;          /* double capacity */
                seq = seq_realloc(seq, seq_cap);
            }
            seq[seq_size++] = (seq_t)next_candidate;
        }

        /* advance to next odd candidate */
        if (next_candidate <= 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    return seq[n - 1];
}
