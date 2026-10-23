/*
 * A172113  Partial sums of the generalized Cuban primes A007645.
 * https://oeis.org/A172113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the first n primes that are 3 or 1 (mod 3)
 *            (equivalently: primes of the form x^2 + xy + y^2)
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* declares bool is_prime(idx_t) */

static seq_t *cum = NULL;          /* cumulative sums of qualifying primes */
static idx_t storedcnt = 0;        /* how many terms currently stored */
static size_t cap = 0;             /* allocated capacity of array cum */
static uint64_t last_checked = 2;  /* last integer examined */

/* Ensure the array has at least ‘newcap’ slots. */
static void grow(void) {
    size_t newcap = cap ? cap * 2 : 32;
    seq_t *newarr = (seq_t *)realloc(cum, newcap * sizeof(seq_t));
    if (!newarr) abort();          /* allocation failed */
    cum = newarr;
    cap = newcap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* harmless, offset is 1 */

    /* Extend the list until at least n terms have been computed. */
    while (storedcnt < n) {
        /* Start searching just beyond the last value tested. */
        uint64_t cand = last_checked + 1;

        /* Advance to the next odd number – all qualifying primes are odd
           except the special case 3, which is handled explicitly below. */
        if (cand % 2 == 0) cand++;

        /* Scan candidates.  Stop as soon as we have enough terms. */
        for (;;) {
            /* A prime qualifies if it is 3 or congruent to 1 mod 3. */
            if (cand == 3 || (cand % 3 == 1)) {
                if (is_prime((idx_t)cand)) {
                    seq_t newsum;
                    if (storedcnt == 0)
                        newsum = (seq_t)cand;
                    else
                        newsum = cum[storedcnt - 1] + (seq_t)cand;

                    if (storedcnt == cap)
                        grow();
                    cum[storedcnt++] = newsum;

                    if (storedcnt >= n) break;   /* enough terms now */
                }
            }

            /* Advance to the next odd candidate. */
            cand += 2;
        }

        /* Remember the last candidate examined so that the next search
           will start just beyond it.  We must capture ‘cand’ itself:
           it is the last prime that was just processed. */
        last_checked = cand;
    }

    return cum[n - 1];
}
