/*
 * A046120  Largest member of a sexy prime triple; value of p+12 where p, p+6
 *          and p+12 are all prime, but p+18 is not.
 * https://oeis.org/A046120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 338, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A046120 – Largest member of a sexy prime triple
 *  a(n) = p + 12 where p, p+6, p+12 are prime, but p+18 is not.
 *
 *  The function uses a simple incremental search for sexy‑prime
 *  triples, memoises found values in a static dynamic array, and
 *  returns a(n) for any n ≥ 1.  It is valid C11 and C++17 and
 *  relies on the supplied pro‑totype utilities.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"          /* bool is_prime(idx_t x); */
#include "../utils/seq_alloc_new_array.h" /* seq_t *seq_alloc_new_array(size_t n); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for offset ≤ 0 */

    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of computed terms */
    static size_t cap     = 0;      /* allocated capacity in terms */
    static size_t count   = 0;      /* number of terms stored */
    static idx_t next_p   = 5;      /* next odd candidate to test */

    /* enlarge cache until it holds at least n terms */
    while (count < n) {
        idx_t p = next_p;

        /* search for the next sexy‑prime triple */
        for (;;) {
            if (is_prime(p) &&
                is_prime(p + 6) &&
                is_prime(p + 12) &&
                !is_prime(p + 18)) {

                /* grow the cache if necessary */
                if (count + 1 > cap) {
                    size_t new_cap = (cap == 0) ? 128 : cap << 1;
                    if (new_cap < count + 1) new_cap = count + 1;

                    /* allocate a new array that properly constructs
                     * any arbitrary‑precision seq_t objects
                     */
                    seq_t *tmp = seq_alloc_new_array(new_cap);
                    for (size_t i = 0; i < count; ++i)
                        tmp[i] = cache[i];
                    free(cache);
                    cache = tmp;
                    cap   = new_cap;
                }

                /* store largest member of the triple */
                cache[count] = (seq_t)(p + 12);
                ++count;

                /* continue searching with the next odd number */
                next_p = p + 2;
                break;
            }
            p += 2;                  /* skip even numbers */
        }
    }

    return cache[n - 1];
}
