/*
 * A108192  Numbers k such that k and the product of the digits of k have at
 *          least one digit in common.
 * https://oeis.org/A108192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 159, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A108192: Numbers k such that k and the product of the digits of k have at least one digit in common. 
   The function returns the n‑th element of this sequence (offset 1).  
   Computation is performed by a brute‑force scan and memoisation of the terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static storage for memoised terms */
    static seq_t *list = NULL;
    static size_t len = 0;      /* number of terms stored */
    static size_t cap = 0;      /* allocated capacity of list */
    static seq_t next_k = 0;    /* smallest candidate to test next */

    /* Ensure we have at least n terms stored */
    while (len < (size_t)n) {
        seq_t k = next_k;
        /* Find the next k that satisfies the property */
        while (1) {
            uint16_t mask_k = 0;   /* mask of digits in k */
            seq_t prod = 1;        /* product of digits of k */

            if (k == 0) {              /* special case k = 0 */
                mask_k = 1 << 0;
                prod = 0;
            } else {
                seq_t tmp = k;
                while (tmp > 0) {
                    uint64_t d = (uint64_t)(tmp % 10);
                    mask_k |= 1 << d;
                    if (d == 0) {
                        prod = 0;
                    } else if (prod != 0) {
                        prod *= d;
                    }
                    tmp /= 10;
                }
            }

            /* Compute mask of digits in the product */
            uint16_t mask_p = 0;
            if (prod == 0) {
                mask_p = 1 << 0;
            } else {
                seq_t ptmp = prod;
                while (ptmp > 0) {
                    uint64_t d = (uint64_t)(ptmp % 10);
                    mask_p |= 1 << d;
                    ptmp /= 10;
                }
            }

            if (mask_k & mask_p) {   /* common digit found */
                break;
            }
            ++k;
        }

        /* Store the found term */
        if (len == cap) {
            size_t new_cap = cap ? cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failed – abort comfortably */
                exit(EXIT_FAILURE);
            }
            list = tmp;
            cap = new_cap;
        }
        list[len++] = k;
        next_k = k + 1;            /* next candidate after this term */
    }

    /* Return the requested term (offset 1) */
    return list[(size_t)(n - 1)];
}
