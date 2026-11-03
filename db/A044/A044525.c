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
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool has06(seq_t n) {
    if (n == 0)
        return false;
    seq_t t = n;
    int digits[64];
    int len = 0;
    while (t > 0) {
        digits[len++] = (int)(t % 7);
        t /= 7;
    }
    for (int i = len - 1; i >= 1; --i) {
        if (digits[i] == 0 && digits[i - 1] == 6)
            return true;
    }
    return false;
}

seq_t a(idx_t n) {
    /* Memoization: store all found terms up to the largest requested n. */
    static seq_t *list = NULL;
    static size_t list_capacity = 0;
    static size_t list_len = 0;
    static seq_t next_candidate = 1;

    if (list == NULL) {
        list_capacity = 256;
        list = mem_new(seq_t, list_capacity);
    }

    while (list_len < n) {
        seq_t m = next_candidate++;
        if (has06(m) && !has06(m + 1)) {
            if (list_len == list_capacity) {
                list_capacity *= 2;
                list = mem_resize(seq_t, list, list_capacity);
            }
            list[list_len++] = m;
        }
    }
    return list[n - 1];
}
