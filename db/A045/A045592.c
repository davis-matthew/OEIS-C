/*
 * A045592  Numbers k that divide 7^k + 4^k.
 * https://oeis.org/A045592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A045592: a(n) = the n-th integer k (k ≥ 1) such that k divides 7^k + 4^k. 
   The function finds each term by brute‑force search over candidates k ≡ 11 (mod 22) (i.e. k = 11·(2·t + 1)), 
   performing modular exponentiation § the other special case k = 1.  
*/

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for 0th term */
        return 0;

    static seq_t *list = NULL;
    static size_t list_size = 0, list_cap = 0;
    static idx_t next_mult = 0; /* generates candidates k = 11 * (2*next_mult + 1) */

    /* initialise cache with first term a(1)=1 */
    if (list_size == 0) {
        list_cap = 16;
        list = (seq_t *)realloc(NULL, list_cap * sizeof(seq_t));
        if (!list) return 0;
        list[0] = (seq_t)1;
        list_size = 1;
    }

    /* generate further terms until we have at least n of them */
    while (list_size < (size_t)n) {
        /* candidate k = 11 * (2*next_mult + 1) */
        idx_t k = 11 * ((idx_t)2 * next_mult + (idx_t)1);

        /* compute 7^k mod k and 4^k mod k */
        seq_t pow7 = mod_pow((seq_t)7, (uint64_t)k, k);
        seq_t pow4 = mod_pow((seq_t)4, (uint64_t)k, k);

        /* compute (pow7 + pow4) mod k without overflow */
        seq_t rem;
        /* sum = pow7 + pow4; if sum ≥ k, subtract k */
        if (pow7 >= k - pow4)      /* k > pow4, so k - pow4 is well defined */
            rem = pow7 - (k - pow4);
        else
            rem = pow7 + pow4;

        if (rem == 0) {            /* test succeeded: k divides 7^k + 4^k */
            if (list_size >= list_cap) {
                size_t new_cap = list_cap ? list_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
                if (!tmp) return 0;
                list = tmp;
                list_cap = new_cap;
            }
            list[list_size++] = (seq_t)k;
        }

        next_mult++;                /* advance to next candidate */
    }

    return list[n - 1];
}
