/*
 * A172484  Partial sums of extravagant numbers, also called prodigal numbers,
 *          or wasteful numbers.
 * https://oeis.org/A172484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum_{k=1..n} A046760(k)
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   digit_count, digits, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    where A046760(k) = k if k is extravagant, otherwise 0 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/digit_count.h" /* digit_count(uint64_t n) */
#include "../utils/digits.h"      /* digits(idx_t x) */

/*
 * Test whether a 64‑bit integer m is extravagant.
 * A number is extravagant if, when written as a product of prime
 * powers, the total number of decimal digits required to write each
 * prime plus, when exponent > 1, its exponent, is greater than
 * the number of decimal digits of m itself.
 */
static bool is_extravagant(uint64_t m)
{
    if (m < 2)
        return false;               /* 1 is not extravagant */

    /* factorise m.  An array of length 64 is plenty for uint64_t. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    size_t D = 0;                    /* digits needed for factorisation */
    for (size_t i = 0; i < cnt; ++i) {
        D += digits(primes[i]);          /* digits of the prime */
        if (exps[i] > 1)
            D += digits(exps[i]);        /* digits of the exponent if >1 */
    }

    size_t l = digit_count(m);          /* digits of m itself */
    return D > l;
}

/* Compute the n‑th partial sum of extravagant numbers. */
seq_t a(idx_t n)
{
    /* static state – works with non‑monotonic calls */
    static seq_t *cum = NULL;    /* cum[0] = 0, cum[k] = a(k) for k>=1 */
    static size_t len  = 0;      /* number of entries stored (len>=1 after init) */
    static size_t cap  = 0;      /* allocated capacity of cum[]   */
    static uint64_t last_checked = 1; /* last integer examined */

    if (n == 0)
        return 0;               /* undefined but harmless */

    /* lazy initialisation */
    if (!cum) {
        cap = 16;
        cum = (seq_t *)malloc(cap * sizeof(seq_t));
        cum[0] = 0;
        len = 1;
    }

    /* grow cum[] until the n‑th sum is available */
    while (len <= (size_t)n) {
        ++last_checked;
        if (is_extravagant(last_checked)) {
            if (len >= cap) {
                size_t new_cap = cap * 2;
                cum = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
                cap = new_cap;
            }
            cum[len] = cum[len - 1] + (seq_t)last_checked;
            ++len;
        }
    }

    return cum[n];
}
