/*
 * A182348  Primes of the form n^4 - 3.
 * https://oeis.org/A182348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A182348 – primes of the form n⁴ – 3
 *
 * The sequence lists primes p that can be written as n⁴ – 3 for some integer n ≥ 2.
 * a(1) = 13 (2⁴ – 3), a(2) = 4093 (8⁴ – 3), ...
 *
 * This implementation searches n in increasing order, tests the primality of
 * n⁴ – 3 with the supplied big-prime routine, and caches the primes
 * found so far.  The cache is resizable and independent of the order of
 * calls, so a(5) can be requested before a(3) without recomputation.
 *
 * The function uses `big_pow` for generic integer exponentiation so that
 * it works even when `seq_t` is an arbitrary‑precision type.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/big_pow.h"     /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/is_prime_big.h"/* bool is_prime_big(seq_t n);          */

/* Resize the dynamic array to hold at least `need` elements. */
static void ensure_capacity(size_t need, size_t *cap, seq_t **arr) {
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!tmp) { /* out of memory */
            /* In contest environment a malloc failure is fatal – abort */
            abort();
        }
        *arr = tmp;
        *cap = newcap;
    }
}

seq_t a(idx_t n) {
    /* Static cache of primes found so far */
    static seq_t *primes = NULL;
    static size_t primes_len = 0;   /* number of primes stored */
    static size_t primes_cap = 0;   /* allocated capacity */
    static idx_t next_n = 2;        /* next integer n to test (n ≥ 2) */

    /* a(0) is undefined – return 0 to avoid crashes if called */
    if (n == 0) return 0;

    /* Expand cache until we have at least n primes */
    while (primes_len < (size_t)n) {
        /* Compute p = next_n⁴ – 3 using generic big_pow to avoid overflow */
        seq_t p = big_pow((seq_t)next_n, (seq_t)4);
        p = p - (seq_t)3;

        if (is_prime_big(p)) {
            /* store prime in cache */
            ensure_capacity(primes_len + 1, &primes_cap, &primes);
            primes[primes_len++] = p;
        }
        ++next_n;
    }

    return primes[n - 1];
}
