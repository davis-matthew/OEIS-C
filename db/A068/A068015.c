/*
 * A068015  Gaps between non-twin primes.
 * https://oeis.org/A068015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007510(n+1) - A007510(n) (gaps between non‑twin primes)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *non_twin = NULL;   /* array of non‑twin primes (seq_t type) */
static size_t non_cnt  = 0;      /* number of primes stored */
static size_t non_cap  = 0;      /* capacity of the array */

/* last prime processed, used to generate the next prime */
static idx_t last_prime = 0;

/* grow the array if we need one more element */
static void grow_if_needed(void)
{
    if (non_cnt >= non_cap) {
        size_t new_cap = (non_cap == 0) ? 16 : non_cap << 1;
        seq_t *tmp = (seq_t *)realloc(non_twin, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        non_twin = tmp;
        non_cap = new_cap;
    }
}

/* generate primes until we have at least n+1 non‑twin primes */
static void ensure_non_twin_up_to(idx_t n)
{
    if ((idx_t)non_cnt > n) return;   /* already enough */

    while ((idx_t)non_cnt <= n) {     /* need indices 0..n inclusive */
        idx_t p;

        /* find next prime */
        if (last_prime == 0) {
            p = 2;
        } else {
            idx_t cand = last_prime + 1;
            if ((cand & 1) == 0) cand++;          /* skip even numbers */
            for (;;) {
                if (is_prime(cand)) break;
                cand += 2;
            }
            p = cand;
        }
        last_prime = p;

        /* Determine if p is a non‑twin prime */
        bool twin = false;
        if (p > 2) {
            if (is_prime(p - 2)) twin = true;
        }
        if (!twin && is_prime(p + 2)) twin = true;

        if (!twin) {
            /* store as seq_t (converts from idx_t) */
            grow_if_needed();
            non_twin[non_cnt++] = (seq_t)p;
        }
    }
}

/* required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 – undefined for n<1 */
    ensure_non_twin_up_to(n);            /* need indices 0..n */
    return non_twin[(size_t)n] - non_twin[(size_t)(n - 1)];
}
