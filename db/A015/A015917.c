/*
 * A015917  Numbers k such that sigma(k) + 12 = sigma(k+12).
 * https://oeis.org/A015917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that sigma(k)+12 = sigma(k+12)
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    static seq_t *list = NULL;
    static size_t list_size = 0;
    static size_t list_cap = 0;
    static idx_t current = 1;  /* start checking from k = 1 */

    if (n == 0) return 0;  /* offset is 1, but guard against 0 */

    /* Generate terms until we have at least n of them */
    while (list_size < n) {
        seq_t sigma_k   = sigma_of(current);
        seq_t sigma_k12 = sigma_of(current + 12);

        if (sigma_k + (seq_t)12 == sigma_k12) {
            if (list_size == list_cap) {
                size_t new_cap = list_cap ? list_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);  /* allocation failure */
                list = tmp;
                list_cap = new_cap;
            }
            list[list_size++] = (seq_t)current;
        }
        ++current;
    }

    return list[n - 1];
}
