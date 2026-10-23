/*
 * A104357  a(n) = A104350(n) - 1.
 * https://oeis.org/A104357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)+1)*A006530(n) - 1, a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here A006530(n) is: the smallest odd prime factor of n,
   or 2 if n is a power of two.   */

#include <stdlib.h>
#include <stdint.h>

/* State maintained between calls */
static idx_t max_computed = 0;          /* largest n for which pref is valid   */
static seq_t *pref         = NULL;      /* pref[n] = product_{k=2..n} A006530(k) */
static uint64_t *factor_arr = NULL;     /* factor_arr[n] = A006530(n) */

static void
ensure_up_to(idx_t n)
{
    if (n <= max_computed)
        return;

    /* Allocate enough room for indices up to n (>= max_computed+1) */
    size_t new_cap = (size_t)(n + 1) * 2;
    seq_t *new_pref    = (seq_t *)realloc(pref,    new_cap * sizeof(seq_t));
    uint64_t *new_factor = (uint64_t *)realloc(factor_arr, new_cap * sizeof(uint64_t));
    if (!new_pref || !new_factor) {
        /* Allocation failed – abort the program */
        abort();
    }
    pref       = new_pref;
    factor_arr = new_factor;

    if (max_computed == 0) {
        /* Initialise base value pref[1] = 1 (product over empty set) */
        pref[1] = (seq_t)1;
        factor_arr[1] = 1;      /* unused for n==1, but kept for symmetry */
        max_computed = 1;
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Compute the factor A006530(i) */
        uint64_t m = (uint64_t)i;
        while ((m & 1ULL) == 0ULL)   /* strip powers of two */
            m >>= 1;

        uint64_t f;
        if (m == 1ULL) {             /* n is a power of two */
            f = 2ULL;
        } else {
            /* largest odd prime factor of the odd part of i */
            uint64_t largest = 1ULL;
            uint64_t t = m;
            for (uint64_t p = 3ULL; p * p <= t; p += 2ULL) {
                if (t % p == 0ULL) {
                    largest = p;
                    while (t % p == 0ULL)
                        t /= p;
                }
            }
            if (t > 1ULL)
                largest = t;
            f = largest;
        }

        factor_arr[i] = f;
        pref[i] = pref[i - 1] * (seq_t)f;
    }

    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* behaviour below offset is arbitrary */

    if (n == 1)
        return (seq_t)0;

    ensure_up_to(n);
    return pref[n] - (seq_t)1;
}
