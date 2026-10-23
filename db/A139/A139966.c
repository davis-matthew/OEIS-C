/*
 * A139966  Primes of the form 7x^2+66y^2.
 * https://oeis.org/A139966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime < 0 such that p ≡ 7,73,145,241,271,391, *
 *            409,439,481,535,601,607,703,745,769,871,937,943,985,1063,1207,1231,
 *            * 1249,1273,1399,1447,1657,1711,1735,1777,1825 (mod 1848)
 * Range:     u8 n <= 3, u16 n <= 425, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"     /* is_prime(idx_t) */
#include "../utils/grow_seq.h"    /* grow_seq(seq_t **, size_t *, size_t) */

static bool representable(seq_t p)
{
    /* Residues modulo 1848 for primes of the form 7x^2+66y^2. */
    static const int residues[] = {7, 73, 145, 241, 271, 391, 409, 439,
                                   481, 535, 601, 607, 703, 745, 769,
                                   871, 937, 943, 985, 1063, 1207,
                                   1231, 1249, 1273, 1399, 1447,
                                   1657, 1711, 1735, 1777, 1825};
    const int count = sizeof residues / sizeof residues[0];
    int r = (int)(p % 1848);
    for (int i = 0; i < count; ++i)
        if (r == residues[i]) return true;
    return false;
}

seq_t a(idx_t n)
{
    /* Static state for memoization */
    static seq_t *seq = NULL;         /* Cached sequence values */
    static size_t len = 0;            /* Number of values stored */
    static size_t cap = 0;            /* Allocated capacity */
    static idx_t candidate = 2;       /* Next integer to test for primality */
    static bool initialized = false; /* Lazy initialization flag */

    if (!initialized) {
        initialized = true;
    }

    while (len < n) {
        /* Find next prime */
        while (!is_prime(candidate)) {
            candidate += (candidate == 2) ? 1 : 2; /* Skip even numbers after 2 */
        }
        seq_t p = (seq_t)candidate;

        /* Check representability */
        if (representable(p)) {
            grow_seq(&seq, &cap, len + 1);
            seq[len++] = p;
        }

        /* Advance candidate: skip even numbers after 2 */
        candidate += (candidate == 2) ? 1 : 2;
    }

    return seq[n - 1];
}
