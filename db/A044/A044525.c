/*
 * A044525  Numbers n such that string 0,6 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑7 representation *
 *            contains the consecutive digits 0 followed by 6, and whose
 *            successor n+1 * does not contain that substring
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has06
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has06.h"

seq_t a(idx_t n) {
    /* Memoization: store all found terms up to the largest requested n. */
    static seq_t *list = NULL;
    static size_t list_capacity = 0;
    static size_t list_len = 0;
    static seq_t next_candidate = 1;

    if (list == NULL) {
        list_capacity = 256;
        list = (seq_t *)malloc(sizeof(seq_t) * list_capacity);
    }

    while (list_len < n) {
        seq_t m = next_candidate++;
        if (has06(m) && !has06(m + 1)) {
            if (list_len == list_capacity) {
                list_capacity *= 2;
                list = (seq_t *)realloc(list, sizeof(seq_t) * list_capacity);
            }
            list[list_len++] = m;
        }
    }
    return list[n - 1];
}
