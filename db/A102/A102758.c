/*
 * A102758  Numbers k such that k3333333 is prime.
 * https://oeis.org/A102758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth integer k such that the decimal number k 3333333
 *            (i.e. k*10^7+3333333) is prime. The sequence is computed by
 *            testing numbers k in increasing order and keeping those that
 *            yield a prime in the required form
 * Range:     u8 n <= 35, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_ull.h"

/* ---------- Miller‑Rabin 64‑bit primality test -------------------------------- */

/* ------------------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Dynamically store found k values */
    static uint64_t *ks = NULL;
    static idx_t ks_size = 0;
    static idx_t ks_cap  = 0;
    static idx_t last_tested = 0;   /* largest k examined so far */

    /* Ensure we can hold n elements */
    if (ks_size < n && ks_cap < n) {
        idx_t new_cap = ks_cap ? ks_cap * 2 : 64;
        if (new_cap < n) new_cap = n;
        uint64_t *tmp = (uint64_t *)realloc(ks, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        ks     = tmp;
        ks_cap = new_cap;
    }

    /* Extend the list until we have n terms */
    if (ks_size < n) {
        const uint64_t SUFFIX = 3333333ULL;
        const uint64_t MULT   = 10000000ULL; /* 10^7 */

        uint64_t k;
        for (k = last_tested + 1; ks_size < n; ++k) {
            uint64_t candidate = k * MULT + SUFFIX;
            if (is_prime_ull(candidate))
                ks[ks_size++] = k;
        }
        last_tested = k - 1;
    }

    /* Convert the stored uint64_t k to seq_t for return */
    return (seq_t)ks[n - 1];
}
