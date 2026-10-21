/*
 * A073669  a(1) = 1, a(n) = the smallest composite multiple of n such that
 *          every partial sum is prime.
 * https://oeis.org/A073669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 353, u32 n <= 62, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1.
   For n > 1, a(n) is the smallest composite multiple of n such that
   the cumulative sum of the sequence up to that point is prime.  The
   function keeps a running table of a(n) and the corresponding sums. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Static storage for the sequence and its running sums. */
static seq_t *a_vals    = NULL;   /* a(n), indexed from 1 */
static seq_t *prefix    = NULL;   /* prefix sums S(n) */
static size_t cap       = 0;      /* allocated capacity (max index) */
static size_t last      = 0;      /* largest n computed so far */

/* Allocate or grow the storage so that indices up to 'need' (inclusive)
   are available.  New entries are left uninitialised. */
static void ensure_cap(size_t need)
{
    if (need <= cap) return;               /* already enough */

    size_t new_cap = (cap == 0) ? 64 : cap;
    while (new_cap < need) new_cap <<= 1;   /* double until sufficient */

    seq_t *new_a    = (seq_t *)malloc(sizeof(seq_t) * (new_cap + 1));
    seq_t *new_pref = (seq_t *)malloc(sizeof(seq_t) * (new_cap + 1));
    if (!new_a || !new_pref) {             /* allocation failed */
        exit(1);
    }

    /* Copy existing data if any. */
    if (a_vals) {
        for (size_t i = 1; i <= last; ++i) {
            new_a[i]    = a_vals[i];
            new_pref[i] = prefix[i];
        }
        free(a_vals);
        free(prefix);
    }

    a_vals = new_a;
    prefix = new_pref;
    cap    = new_cap;
}

/* Initialise the first term lazily. */
static void init_sequence(void)
{
    if (a_vals) return;

    ensure_cap(1);
    a_vals[1]  = (seq_t)1;
    prefix[1]  = (seq_t)1;
    last = 1;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;     /* undefined for n <= 0 */
    init_sequence();

    /* Extend the table up to n if necessary. */
    if ((size_t)n > last) {
        ensure_cap((size_t)n);
        for (idx_t k = last + (idx_t)1; k <= n; ++k) {
            seq_t sum_prev = prefix[k - 1];
            seq_t m = (seq_t)k;                 /* first multiple of k */
            bool found = false;
            while (!found) {
                /* m must be composite (>1 and not prime). */
                if (m > (seq_t)1 && !is_prime_big(m)) {
                    seq_t new_sum = sum_prev + m;
                    if (is_prime_big(new_sum)) {
                        a_vals[(size_t)k]  = m;
                        prefix[(size_t)k]  = new_sum;
                        last              = (size_t)k;
                        found             = true;
                        break;
                    }
                }
                m += (seq_t)k;   /* next multiple of k */
            }
        }
    }

    return a_vals[(size_t)n];
}
