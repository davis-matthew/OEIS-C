/*
 * A179780  Primes, q, such that for three consecutive primes, p, q & r, with
 *          p<q<r, neither (q - p)/(r - q) nor (r - q)/(q - p) is an integer.
 * https://oeis.org/A179780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes q such that for consecutive primes p<q<r,
   neither (q-p)/(r-q) nor (r-q)/(q-p) is an integer. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* External integer types provided by the build system. */
seq_t a(idx_t n)
{
    /* Memoisation state. */
    static seq_t *seq      = NULL;   /* output sequence */
    static size_t seq_cap  = 0;      /* current capacity of seq */
    static idx_t seq_len   = 0;      /* number of elements stored */

    static uint64_t *primes = NULL;  /* generated primes (stored as uint64_t) */
    static size_t primes_cap  = 0;    /* capacity of primes array */
    static idx_t primes_cnt   = 0;    /* number of primes stored */

    static idx_t mid_index = 1; /* index of the middle prime in current triple*/

    /* Initialise state on first call. */
    if (!seq) {
        seq_cap = 256;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));

        primes_cap = 64;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2u;
        primes[1] = 3u;
        primes_cnt = 2;
        mid_index = 1; /* initial triple: 2,3,5 */
    }

    /* Generate new terms until we have at least n of them. */
    while (seq_len < n) {
        /* Ensure we have the triple p,q,r available. */
        while (primes_cnt <= mid_index + 1) {
            idx_t cand = (idx_t)primes[primes_cnt - 1] + 2;
            while (!is_prime(cand)) {
                cand += 2;
            }
            /* Store the new prime. */
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
                primes = tmp;
            }
            primes[primes_cnt++] = (uint64_t)cand;
        }

        uint64_t p = primes[mid_index - 1];
        uint64_t q = primes[mid_index];
        uint64_t r = primes[mid_index + 1];

        uint64_t g1 = q - p;
        uint64_t g2 = r - q;

        /* The ratio is not integral if neither division yields an integer. */
        if ((g1 % g2) != 0 && (g2 % g1) != 0) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
                seq = tmp;
            }
            seq[seq_len++] = (seq_t)q; /* store as seq_t */
        }

        mid_index++; /* Move to next triple. */
    }

    return seq[n - 1];
}
