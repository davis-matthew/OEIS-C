/*
 * A147578  Numbers with exactly 5 distinct odd prime divisors {3,5,7,11,13}.
 * https://oeis.org/A147578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15015 * H_n where H_n is the nth smallest number whose
 *            prime factors are only 3,5,7,11,13. This sequence gives the
 *            numbers with exactly the five distinct odd primes {3,5,7,11,13}
 * Range:     u8 none, u16 n <= 2, u32 n <= 589, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The five allowed odd primes */
    static const seq_t primes[5] = {3, 5, 7, 11, 13};
    static const seq_t base = 15015;              /* product of the five primes */

    /* The Hamming‑type list H, 1‑based.  H[1] == 1. */
    static seq_t *H = NULL;
    static size_t H_cap = 0;        /* allocated capacity, 1‑based */
    static size_t H_len = 0;        /* current filled length (>=1) */

    /* Indices for generating the next candidate from each prime. */
    static idx_t idx[5] = {1, 1, 1, 1, 1};

    /* Ensure the list H is built up to the desired index n. */
    if (n == 0) return 0;  /* undefined for n = 0, keep crash safe */

    /* Initialize the list on first call. */
    if (H_len == 0) {
        H_cap = 64;
        H = (seq_t *)malloc(H_cap * sizeof(seq_t));
        if (H == NULL) return 0;       /* allocation failure */
        H[1] = (seq_t)1;
        H_len = 1;
    }

    /* Grow the array if necessary. */
    if ((size_t)n > H_len) {
        size_t target = (size_t)n;
        if ((size_t)H_cap <= target) {
            size_t new_cap = H_cap * 2;
            while ((size_t)new_cap <= target) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(H, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;   /* allocation failure */
            H = tmp;
            H_cap = new_cap;
        }

        /* Generate the next Hamming numbers until H_len == target. */
        while (H_len <= target) {
            seq_t next = primes[0] * H[idx[0]];
            for (int i = 1; i < 5; ++i) {
                seq_t val = primes[i] * H[idx[i]];
                if (val < next) next = val;
            }
            H[++H_len] = next;

            /* Advance the indices that produced the minimum. */
            for (int i = 0; i < 5; ++i) {
                if (primes[i] * H[idx[i]] == next) idx[i]++;
            }
        }
    }

    return base * H[(size_t)n];
}
