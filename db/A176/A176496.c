/*
 * A176496  a(n) = Sum_{k=1..n} 2^nonprime(k).
 * https://oeis.org/A176496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} 2^A018252(k) where A018252(k) is the k-th
 *            nonprime integer (1,4,6,8,9,10,12,14,...)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 45, u128 n <= 96,
 *            big any n
 * Helpers:   is_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* static memoization state */
    static seq_t *cum   = NULL; /* cumulative sums a(1..k) */
    static size_t  cum_len = 0;
    static size_t  cum_cap = 0;

    static idx_t  last_k   = 0;   /* number of nonprimes processed so far */
    static uint64_t last_num = 1; /* next integer to test for nonprime */

    /* if already computed up to n, return it */
    if (cum_len >= n)
        return cum[n - 1];

    while (last_k < n)
    {
        /* Determine if last_num is a nonprime (1 or composite) */
        bool nonprime = (last_num == 1) || !is_prime(last_num);

        if (nonprime)
        {
            ++last_k;  /* we have found the next nonprime number */

            seq_t term = pow2((idx_t)last_num);
            seq_t sum  = term;
            if (cum_len > 0)
                sum = cum[cum_len - 1] + term;  /* cumulative sum */

            /* Grow the array if needed */
            if (cum_len == cum_cap)
            {
                size_t new_cap = cum_cap ? cum_cap * 2 : 4;
                seq_t *new_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
                if (!new_arr) abort();
                if (cum_len)
                    memcpy(new_arr, cum, cum_len * sizeof(seq_t));
                free(cum);
                cum     = new_arr;
                cum_cap = new_cap;
            }
            cum[cum_len++] = sum;
        }
        ++last_num;  /* advance to next integer */
    }

    return cum[n - 1];
}
