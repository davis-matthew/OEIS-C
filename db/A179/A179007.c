/*
 * A179007  Sum of 3 consecutive composite odd numbers.
 * https://oeis.org/A179007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n) + C(n+1) + C(n+2), where C(k) is the k‑th odd
 *            composite number
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Cached list of odd composite numbers. */
static seq_t *comps      = NULL;
static size_t comps_len  = 0;
static size_t comps_cap  = 0;
static idx_t next_odd    = 3;      /* next odd candidate to test */

/* Append the next odd composite to the cache. */
static void add_next_comp(void)
{
    while (true) {
        idx_t cand = next_odd;
        next_odd += 2U;                 /* advance to the next odd */
        if (cand > 2U && !is_prime(cand)) {
            if (comps_len == comps_cap) {
                size_t new_cap = comps_cap ? comps_cap * 2U : 16U;
                seq_t *tmp = (seq_t *)realloc(comps, new_cap * sizeof(seq_t));
                if (!tmp) abort();      /* allocation failure */
                comps = tmp;
                comps_cap = new_cap;
            }
            comps[comps_len++] = (seq_t)cand;
            break;
        }
    }
}

/* Ensure the cache contains at least `need` composite numbers. */
static void ensure_comps_size(size_t need)
{
    while (comps_len < need) {
        add_next_comp();
    }
}

/* Return the a(n) term of OEIS sequence A179007. */
seq_t a(idx_t n)
{
    if (n < 1U) {
        return 0;                      /* undefined for n < 1 */
    }

    /* Need composites at indices n-1, n, and n+1 (0‑based). */
    size_t required = (size_t)n + 2U;   /* ensures indices 0 .. n+1 inclusive */
    ensure_comps_size(required);

    size_t i = (size_t)(n - 1U);        /* first composite of the triple */
    return comps[i] + comps[i + 1] + comps[i + 2];
}
