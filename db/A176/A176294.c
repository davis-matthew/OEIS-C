/*
 * A176294  Numbers k such that sum of digits of k = sum of digits of k-th
 *          positive nonprime.
 * https://oeis.org/A176294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 3465, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A176294: numbers k such that sum of digits of k equals sum of digits of k‑th positive nonprime (OEIS A176294) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* digit_sum: technology available for seq_t */
#include "../utils/digit_sum.h"

/* helper: test if an unsigned 64‑bit integer is prime by simple trial division */
static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* dynamic vector of nonprime numbers */
static uint64_t *nonprimes = NULL;   /* array of uint64_t storing the nonprime sequence */
static size_t np_cnt = 0;            /* number of elements in the array */
static size_t np_cap = 0;            /* current capacity */
static uint64_t last_checked = 0;    /* largest number that has been examined */

/* ensure that we have at least k nonprime numbers stored */
static void ensure_np_cnt(size_t k)
{
    while (np_cnt < k) {
        /* extend the array if needed */
        if (np_cnt >= np_cap) {
            size_t new_cap = np_cap == 0 ? 16 : np_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(nonprimes, new_cap * sizeof(uint64_t));
            if (!tmp) exit(1);            /* out of memory */
            nonprimes = tmp;
            np_cap = new_cap;
        }
        /* check the next integer */
        ++last_checked;
        if (!is_prime_u64(last_checked))
            nonprimes[np_cnt++] = last_checked;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    size_t found = 0;      /* how many qualifying k have been seen */
    idx_t   k = 1;         /* candidate k */

    while (true) {
        seq_t ds_k = digit_sum((seq_t)k);         /* digit sum of k */
        ensure_np_cnt(k);                        /* guarantee nonprimes[k] exists */
        seq_t ds_m = digit_sum((seq_t)nonprimes[k-1]);   /* digit sum of kth nonprime */

        if (ds_k == ds_m) {                     /* k qualifies */
            ++found;
            if (found == n)             /* desired term found */
                return (seq_t)k;
        }
        ++k;                                   /* try next k */
    }
}
