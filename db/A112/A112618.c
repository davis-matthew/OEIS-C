/*
 * A112618  Let T(n) = A000073(n+1), n >= 1; a(n) = smallest k such that
 *          prime(n) divides T(k).
 * https://oeis.org/A112618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that prime(n) divides the tribonacci
 *            number T(k)
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 984 (2026-11-01)
 * Helpers:   is_prime_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where T(1)=T(2)=1, T(3)=2, and T(i)=T(i-1)+T(i-2)+T(i-3) for i>3. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_trial.h"

/*--- dynamic array helpers for primes --------------------------------------*/
static void ensure_primes_cap(size_t *cap, uint64_t **arr, size_t min_cap) {
    if (*cap < min_cap) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t get_prime(uint64_t idx) {
    static uint64_t *prime_vec = NULL;
    static size_t prime_cap = 0;
    static size_t prime_sz  = 0;
    static uint64_t candidate = 3;      /* next odd to test */

    if (!prime_vec) { /* first call: initialise */
        ensure_primes_cap(&prime_cap, &prime_vec, 16);
        prime_vec[0] = 2;
        prime_sz = 1;
    }

    while (prime_sz < idx) {
        /* find next prime */
        while (!is_prime_trial(candidate, prime_vec, prime_sz))
            candidate += 2;

        /* add found prime */
        if (prime_sz == prime_cap)
            ensure_primes_cap(&prime_cap, &prime_vec, prime_cap << 1);

        prime_vec[prime_sz++] = candidate;
        candidate += 2;
    }
    return (seq_t)prime_vec[idx - 1];
}

static inline uint64_t mod_add3(uint64_t a, uint64_t b, uint64_t c, uint64_t mod)
{
#if defined(__SIZEOF_INT128__)
    unsigned __int128 sum = (unsigned __int128)a + (unsigned __int128)b + (unsigned __int128)c;
    return (uint64_t)(sum % (unsigned __int128)mod);
#else
    /* Add a and b with overflow detection */
    uint64_t sum = a + b;
    if (sum < a)               /* overflow */
        sum -= mod;
    if (sum >= mod)
        sum -= mod;

    /* Add c with overflow detection */
    sum += c;
    if (sum < c)               /* overflow */
        sum -= mod;
    if (sum >= mod)
        sum -= mod;

    return sum;
#endif
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for indices < 1, but return 0 */

    /* Retrieve the nth prime (1‑indexed) */
    uint64_t p = (uint64_t)get_prime(n);

    /* Handle trivial primes (none for primes) */
    if (p <= 1)
        return (seq_t)1;

    /* Tribonacci numbers modulo p */
    uint64_t t1 = 1 % p;   /* T(1) */
    uint64_t t2 = 1 % p;   /* T(2) */
    uint64_t t3 = 2 % p;   /* T(3) */

    /* Check the first three terms */
    if (t1 == 0)   return (seq_t)1;
    if (t2 == 0)   return (seq_t)2;
    if (t3 == 0)   return (seq_t)3;

    idx_t k = 3;   /* current index for T(k) */

    for (;;) {
        uint64_t t4 = mod_add3(t1, t2, t3, p);   /* T(k+1) */
        ++k;
        if (t4 == 0)
            return (seq_t)k;   /* found minimal k */

        /* Shift for next iteration */
        t1 = t2;
        t2 = t3;
        t3 = t4;
    }
}
