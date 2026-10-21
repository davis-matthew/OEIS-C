/*
 * A073766  a(n) = binomial(composite(n+1), composite(n)) =
 *          binomial(A002808(n+1), A002808(n)).
 * https://oeis.org/A073766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(composite(n+1), composite(n))
 * Range:     u8 n <= 8, u16 n <= 292, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the nth term of OEIS sequence A073766.
 *
 *  composite(k) is the k‑th composite number (4,6,8,9,10,12,…).
 *  We build the list of composites incrementally, memoizing it in a
 *  static array that grows as needed.  The binomial coefficient is
 *  computed with the supplied utility function `binomial(idx_t n,
 *  idx_t k)` which returns a value of type `seq_t`.  The primality
 *  test `is_prime(idx_t)` from the utils library is used to filter
 *  composites.
 *
 *  The function is fully self‑contained, uses only integer arithmetic,
 *  and respects the type requirements from the build environment.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>              /* for size_t */

/* utilities from the provided library */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x);               */
#include "../utils/binomial.h"  /* seq_t binomial(idx_t n, idx_t k);    */

static idx_t *composites = NULL;   /* array of composite numbers */
static size_t comps_len   = 0;     /* how many composites stored   */
static size_t comps_cap   = 0;     /* allocated capacity            */
static idx_t  last_checked = 3;    /* last integer examined (start 4) */

/* Ensure that composites array contains at least `k` entries. */
static void ensure_composites(idx_t k)
{
    while (comps_len < k) {
        ++last_checked;
        /* skip numbers < 4 – although last_checked starts at 3 it may be
         * incremented to 4 on the first iteration */
        if (last_checked < 4)
            continue;

        if (!is_prime(last_checked)) { /* composite number */
            if (comps_len == comps_cap) {
                size_t new_cap = comps_cap ? comps_cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(composites,
                                              new_cap * sizeof(idx_t));
                if (!tmp)
                    abort(); /* memory failure */
                composites = tmp;
                comps_cap  = new_cap;
            }
            composites[comps_len++] = last_checked;
        }
    }
}

/* Return the k‑th composite number (k ≥ 1). */
static idx_t get_composite(idx_t k)
{
    ensure_composites(k);
    return composites[k - 1];
}

/* Main sequence function requested by the test harness. */
seq_t a(idx_t n)
{
    /* n is 1‑based per OEIS definition */
    idx_t c_n  = get_composite(n);     /* composite(n)   */
    idx_t c_n1 = get_composite(n + 1); /* composite(n+1) */
    return binomial(c_n1, c_n);
}
