/*
 * A070079  a(n) gives the odd leg of the unique primitive Pythagorean
 *          triangle with hypotenuse A002144(n).
 * https://oeis.org/A070079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = u² – v² where p = A002144(n) = u² + v² is the n‑th prime
 *            ≡1 (mod 4), the odd leg of the primitive Pythagorean triangle
 *            with hypotenuse p
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _root_neg1, floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"
#include "../utils/_root_neg1.h"

#define MAX_N 20000          /* enough for n up to about 10000 */
static idx_t primes1[MAX_N];  /* primes ≡1 (mod 4) */
static seq_t legs[MAX_N];     /* corresponding odd legs */
static size_t len = 0;        /* number of stored entries */
static idx_t next = 5;        /* next candidate to test */

/* Compute the odd leg for a prime p ≡1 (mod 4) using Cornacchia's algorithm. */
static seq_t _odd_leg_for_prime(idx_t p)
{
    idx_t a = _root_neg1(p);
    seq_t r0 = (seq_t)p;
    seq_t r1 = (seq_t)a;

    while (r1 * r1 > (seq_t)p) {
        seq_t r0_new = r1;
        seq_t r1_new = r0 % r1;
        r0 = r0_new;
        r1 = r1_new;
    }

    seq_t y_sqr = (seq_t)p - r1 * r1;
    seq_t y = floor_sqrt(y_sqr);
    if (y * y != y_sqr)
        return 0;  /* failure – should not occur for valid primes */

    seq_t u = (r1 >= y) ? r1 : y;
    seq_t v = (r1 >= y) ? y : r1;
    return u * u - v * v;  /* u² – v² */
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    while (len < n) {
        if ((next % 4 == 1) && is_prime(next)) {
            if (len == MAX_N) /* safety guard: should not happen for n ≤ 10000 */
                abort();
            primes1[len] = next;
            legs[len]    = _odd_leg_for_prime(next);
            ++len;
        }
        next += 2;  /* skip even numbers */
    }

    return legs[n - 1];
}
