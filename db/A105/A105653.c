/*
 * A105653  Numbers n such that p1=2n+3, p2=4n+5, p3=6n+7 and p4=8n+9 are all
 *          prime.
 * https://oeis.org/A105653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that 2n+3, 4n+5, 6n+7 and 8n+9 are all prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Cache of previously found terms. */
    static seq_t *vals = NULL;
    static idx_t sz   = 0;      /* number of terms currently stored */
    static size_t cap = 0;      /* allocated capacity */

    /* Storage for the next candidate to test, kept mod 3 = 2. */
    static uint64_t cand = 2;    /* first admissible n (2 mod 3) */

    /* The function is undefined for n == 0. Returning 0 is harmless. */
    if (n == 0) return 0;

    /* Grow the cache until it contains at least n terms. */
    while (sz < n)
    {
        /* Allocate or enlarge the array as needed. */
        if (sz == 0) {
            cap = 64;
            vals = (seq_t *)malloc(cap * sizeof(seq_t));
        } else if ((size_t)sz == cap) {
            cap <<= 1;
            vals = (seq_t *)realloc(vals, cap * sizeof(seq_t));
        }

        /* Compute the four linear forms using 64‑bit arithmetic. */
        uint64_t p1 = 2 * cand + 3ULL;
        uint64_t p2 = 4 * cand + 5ULL;
        uint64_t p3 = 6 * cand + 7ULL;
        uint64_t p4 = 8 * cand + 9ULL;

        /* Test primality of the four numbers. */
        if (is_prime(p1) && is_prime(p2) && is_prime(p3) && is_prime(p4)) {
            vals[sz++] = (seq_t)cand;   /* store the found n */
        }

        cand += 3;      /* next candidate with n ≡ 2 (mod 3) */
    }

    return vals[n - 1];
}
