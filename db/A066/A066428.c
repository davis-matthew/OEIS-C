/*
 * A066428  Numbers with mu = 0 and infinitary MoebiusMu = +1 (sum of binary
 *          digits of prime exponents is even).
 * https://oeis.org/A066428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066428: Numbers with μ(n)=0 (not square‑free)
   and Σ popcount(exponent of p) over all prime factors p is even. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* popcount for 64‑bit values */
static int popcnt64(uint64_t x) {
    int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

/* Cached sequence values (enough for n up to ~100000) */
static seq_t cache[200000];
static size_t cache_len = 0;

/* Primes for trial division */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_inited = false;

/* Last integer that was examined */
static uint64_t last_checked = 0;

/* Computes the n‑th term of the sequence (1‑based indexing) */
seq_t a(idx_t n) {
    /* Ensure enough cache space, compute until cache_len >= n */
    if (n > sizeof(cache)/sizeof(cache[0])) {
        /* sequence requested beyond pre-allocated size – return 0 as placeholder */
        return (seq_t)0;
    }

    /* Initialise prime list once */
    if (!primes_inited) {
        size_t limit = 200000; /* sufficient for the required range */
        build_primes_upto(limit, &primes, &primes_cnt);
        primes_inited = true;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++last_checked;          /* next integer to test */

        uint64_t tmp = last_checked;
        bool has_square = false;
        int parity = 0;          /* 0 = even, 1 = odd */

        /* Trial division using precomputed primes */
        for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
            uint64_t p = primes[i];
            if (tmp % p == 0) {
                uint64_t e = 0;
                do {
                    tmp /= p;
                    ++e;
                } while (tmp % p == 0);
                if (e >= 2) has_square = true;
                parity ^= (popcnt64(e) & 1);
            }
        }

        /* Remaining prime factor, if any */
        if (tmp > 1) {
            parity ^= 1;  /* exponent 1 → popcount 1 */
        }

        /* Accept if μ(n)=0 (has a square factor) and parity is even */
        if (has_square && parity == 0) {
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[(size_t)n - 1];
}
