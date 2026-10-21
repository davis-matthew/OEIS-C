/*
 * A240713  Number of decompositions of 2n=p1+p2 (prime p1 <= p2), where at
 *          least one other such pair 2n=p3+p4 (prime p3 <= p4) exists such
 *          that |p1-p3|= 6 or 12.
 * https://oeis.org/A240713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of Goldbach decompositions p1 ≤ p2 of 2n such that
 *            there exists another decomposition with |p1 − p3| equal to 6 or
 *            12
 * Range:     u8 n <= 8084, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes and prototypes from the utilities repository */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the sequence is defined to be 0 */
    if (n < 2) return (seq_t)0;

    idx_t two_n = n * 2;

    /* Dynamic array to hold the first primes of the Goldbach pairs */
    size_t cap = 32;
    idx_t *firsts = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!firsts) return (seq_t)0;        /* memory failure – returns 0 */
    size_t len = 0;

    /* Generate all Goldbach decompositions 2n = p + q with p <= q */
    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime(p)) continue;
        idx_t q = two_n - p;
        if (q < p) break;               /* since p increases, q will only decrease */
        if (!is_prime(q)) continue;

        if (len == cap) {
            cap <<= 1;                  /* double capacity */
            idx_t *tmp = (idx_t *)realloc(firsts, cap * sizeof(idx_t));
            if (!tmp) { free(firsts); return (seq_t)0; } /* memory failure */
            firsts = tmp;
        }
        firsts[len++] = p;
    }

    /* Count pairs that have at least one other pair with prime difference
       6 or 12.  Each qualifying pair is counted once. */
    seq_t count = 0;
    for (size_t i = 0; i < len; ++i) {
        idx_t pi = firsts[i];
        bool ok = false;
        for (size_t j = 0; j < len && !ok; ++j) {
            if (i == j) continue;
            idx_t diff = pi > firsts[j] ? pi - firsts[j] : firsts[j] - pi;
            if (diff == 6 || diff == 12) ok = true;
        }
        if (ok) ++count;
    }

    free(firsts);
    return count;
}
