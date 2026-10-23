/*
 * A191032  Primes p with Kronecker symbol (p|46) = 1.
 * https://oeis.org/A191032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A191032:  primes p with Kronecker symbol (p|46) = 1
 *  (i.e. primes that are quadratic residues modulo 46)
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/powmod64.h"
#include "../utils/append_candidate.h"

/* Return the n-th prime p such that the Kronecker symbol (p|46) equals 1. */
/* idx_t is an unsigned integer type for indices; seq_t is an unsigned integer
   or big‑integer type for values. */
seq_t a(idx_t n)
{
    /* static state for memoization */
    static idx_t      cur = 3;          /* next odd candidate to test */
    static seq_t *    list = NULL;      /* array of found primes */
    static size_t     list_cap = 0;     /* capacity of list */
    static size_t     list_len = 0;     /* number of primes stored */

    /* if we already have the n-th term, return it */
    if (n <= list_len)
        return list[n - 1];

    /* continue searching until the n-th term is found */
    while (list_len < (size_t)n) {
        /* test whether cur is prime */
        if (is_prime(cur)) {
            /* ignore the factor p = 23, which gives zero Kronecker symbol */
            if (cur != 23u) {
                /* compute the Legendre symbols (cur|2) and (cur|23) */
                int s2 = ((cur % 8u) == 1u || (cur % 8u) == 7u) ? 1 : -1;

                uint64_t s23_res = powmod64(cur, 11u, 23u);
                int s23 = (s23_res == 1u) ? 1 : ((s23_res == 22u) ? -1 : 0);

                if (s23 != 0 && s2 * s23 == 1) {
                    /* store the prime */
                    append_candidate(&list, &list_len, &list_cap, (seq_t)cur);
                }
            }
        }
        cur += 2;  /* only odd numbers need be checked */
    }

    return list[n - 1];
}
