/*
 * A226934  Numbers k such that tau(2k) | sigma(2k).
 * https://oeis.org/A226934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A226934: Numbers k such that tau(2k) | sigma(2k). */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *list      = NULL;
    static size_t list_cap  = 0;
    static size_t list_len  = 0;
    static idx_t cur = 1;    /* current k being tested */

    /* generate terms until we have at least n terms cached */
    while (list_len < n) {
        uint64_t two_k = ((uint64_t)cur) << 1;  /* 2k */

        /* tau(2k) */
        size_t tau = divisor_count_u64(two_k);

        /* sigma(2k) */
        seq_t sigma = sigma_of(two_k);

        /* if sigma is divisible by tau, store k */
        if (sigma % (seq_t)tau == 0) {
            if (list_len == list_cap) {
                size_t new_cap = list_cap == 0 ? 8 : list_cap << 1;
                seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failed – abort */
                    exit(1);
                }
                list = tmp;
                list_cap = new_cap;
            }
            list[list_len++] = (seq_t)cur;
        }

        /* advance to next candidate */
        cur++;
    }

    /* indices in OEIS start at 1 */
    return list[n - 1];
}
