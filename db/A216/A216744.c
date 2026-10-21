/*
 * A216744  Primes p such that x^26 = -2 has no solution mod p.
 * https://oeis.org/A216744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p (p ≥ 3) for which the congruence * x^26
 *            ≡ -2 (mod p) has no solution. This is equivalent to *
 *            (-2)^((p-1)/gcd(26,p-1)) ≠ 1 (mod p). The function computes the
 *            * sequence by sieving primes and testing the criterion, with
 *            memoisation * so calls in any order are fast
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Euclidean gcd for uint64_t */
static idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* static state for memoised list of primes satisfying the criterion */
    static seq_t *list = NULL;
    static size_t len = 0;      /* number of stored primes */
    static size_t cap = 0;      /* capacity of allocated array */
    static idx_t next = 3;      /* next odd candidate to test (skip 2) */

    /* initialise array on first call */
    if (cap == 0) {
        cap = 16;
        list = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!list) exit(1);      /* guaranteed enough memory for test harness */
    }

    /* generate primes until we have at least n of them */
    while (len < n) {
        idx_t p = next;
        next += 2;               /* skip even numbers after 2 */

        if (!is_prime(p))
            continue;           /* not a prime – skip */

        /* compute (-2)^((p-1)/gcd(26,p-1)) mod p */
        idx_t g    = gcd_u64(26, p - 1);
        idx_t exp  = (p - 1) / g;
        seq_t base = (seq_t)(p - 2);            /* -2 mod p */
        seq_t mod  = (seq_t)p;

        seq_t res = mod_pow(base, (idx_t)exp, mod);

        if (res != (seq_t)1) {                /* prime satisfies the condition */
            if (len == cap) {                 /* grow array if needed */
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(list, cap * sizeof(seq_t));
                if (!tmp) exit(1);
                list = tmp;
            }
            list[len++] = (seq_t)p;
        }
    }

    /* return the requested term (1‑based index) */
    return list[n - 1];
}
