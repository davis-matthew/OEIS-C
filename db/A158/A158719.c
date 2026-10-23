/*
 * A158719  Primes p such that p1 = floor(p/2)+p is not prime and p2 =
 *          ceiling(p/2)+p is not prime, p3 = floor(p1/2)+p1 is not prime and
 *          p5 = ceiling(p1/2)+p1 is not prime, p4 = floor(p2/2)+p2 is not
 *          prime and p6 = ceiling(p2/2)+p2 is not prime.
 * https://oeis.org/A158719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence of primes p such that all of p1 = floor(p/2)+p p2 =
 *            ceil (p/2)+p p3 = floor(p1/2)+p1 p5 = ceil (p1/2)+p1 p4 =
 *            floor(p2/2)+p2 p6 = ceil (p2/2)+p2 are not prime
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static bool satisfies(seq_t p)
{
    /* p1 and p2 */
    seq_t p1 = (3 * p - 1) / 2;
    seq_t p2 = (3 * p + 1) / 2;

    if (is_prime((idx_t)p1) || is_prime((idx_t)p2))
        return false;

    /* p3, p5 from p1 */
    seq_t p3 = p1 + p1 / 2;           /* floor division */
    seq_t p5 = p1 + (p1 + 1) / 2;     /* ceil division */

    /* p4, p6 from p2 */
    seq_t p4 = p2 + p2 / 2;
    seq_t p6 = p2 + (p2 + 1) / 2;

    if (is_prime((idx_t)p3) || is_prime((idx_t)p4) ||
        is_prime((idx_t)p5) || is_prime((idx_t)p6))
        return false;

    return true;
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t size = 0;
    static size_t cap  = 0;
    static seq_t cur   = 3;           /* start with the first odd prime */

    while (size < n) {
        if (is_prime((idx_t)cur) && satisfies(cur)) {
            if (size == cap) {
                size_t new_cap = cap ? cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq = tmp;
                cap = new_cap;
            }
            seq[size++] = cur;
        }
        cur += 2;          /* skip even numbers */
    }
    return seq[n - 1];
}
