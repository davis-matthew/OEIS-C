/*
 * A208177  Primes of the form 128*k + 1.
 * https://oeis.org/A208177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p ≡ 1 (mod 128) Computed by
 *            incremental prime search using next_prime
 * Range:     u8 none, u16 n <= 99, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

static seq_t *res_list = NULL; /* primes congruent to 1 mod 128 */
static size_t res_cap  = 0;
static size_t res_sz   = 0;

/* Current base to start the next search.  We always keep it odd. */
static uint64_t cur_start = 3;

/* Grow re‑allocation of the array that holds the results. */
static void grow_res(void)
{
    size_t new_cap = res_cap ? res_cap * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(res_list, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* fatal allocation failure */
    res_list = tmp;
    res_cap = new_cap;
}

/* Ensure that at least 'need' elements are stored in res_list.  */
static void ensure_res(idx_t need)
{
    if (!res_list) grow_res();

    while (res_sz < (size_t)need) {
        /* next_prime returns the smallest prime > start, so use cur_start-1 */
        uint64_t p = next_prime(cur_start - 1);

        if ((p & 127) == 1) {               /* p % 128 == 1 */
            if (res_sz >= res_cap) grow_res();
            res_list[res_sz++] = (seq_t)p;
        }

        /* move on to the next odd number after p */
        cur_start = p + 2;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* sequence offset is 1 */
    ensure_res(n);
    return res_list[n - 1];
}
