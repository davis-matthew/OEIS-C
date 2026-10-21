/*
 * A075066  Alternately odd and even composite numbers complementing the
 *          parity of the index.
 * https://oeis.org/A075066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   if n is odd: a(n) = the ((n+1)/2)-th odd composite; * if n is
 *            even: a(n) = 2*((n/2)+1) (the even composite sequence)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* WIP: cache of odd composite numbers */
static seq_t *odd_comps = NULL;
static size_t   odd_len  = 0;
static size_t   odd_cap  = 0;
static seq_t   odd_next   = 9;          /* first odd composite */

/* Ensure that at least `need` odd composites are stored. */
static void ensure_odd(size_t need)
{
    if (odd_len >= need) return;

    while (odd_len < need) {
        if (!is_prime_big(odd_next)) {      /* composite */
            if (odd_len == odd_cap) {
                size_t new_cap = (odd_cap == 0) ? 16 : odd_cap << 1;
                seq_t *tmp = (seq_t *)realloc(odd_comps, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                odd_comps = tmp;
                odd_cap   = new_cap;
            }
            odd_comps[odd_len++] = odd_next;
        }
        odd_next += (seq_t)2;               /* next odd candidate */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* out of sequence range */

    if (n & 1) {                             /* odd index → odd composite */
        size_t k = (size_t)((n + 1) >> 1);
        ensure_odd(k);
        return odd_comps[k - 1];
    } else {                                 /* even index → even composite */
        size_t k = (size_t)(n >> 1);        /* floor(n/2) */
        seq_t kk = (seq_t)(k + 1);          /* (n/2)+1 */
        return kk << 1;                     /* 2*((n/2)+1) */
    }
}
