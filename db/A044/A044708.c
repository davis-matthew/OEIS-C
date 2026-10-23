/*
 * A044708  Numbers n such that string 8,4 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer whose base‑9 representation contains
 *            the substring "84" while n+1 does not
 * Range:     u8 n <= 3, u16 n <= 889, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if x in base 9 contains the adjacent digits 8 then 4. */
static bool has84(seq_t x) {
    if (x == 0) return false;
    int digits[32];          /* > 32 enough for any 64‑bit integer          */
    int sz = 0;
    while (x > 0) {
        digits[sz++] = (int)(x % 9);   /* remainder is 0..8                    */
        x = x / 9;
    }
    /* Scan from most significant to least significant digit. */
    for (int i = sz - 1; i > 0; --i) {
        if (digits[i] == 8 && digits[i - 1] == 4) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* behaviour below the offset is irrelevant */

    static seq_t *terms = NULL;      /* memoised terms, 1‑based index */
    static size_t cap   = 0;
    static idx_t  count = 0;         /* number of terms stored */
    static seq_t  cur   = 1;         /* next candidate to test */

    while (count < n) {
        if (has84(cur) && !has84(cur + 1)) {
            if (count >= cap) {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                terms = tmp;
                cap = newcap;
            }
            terms[count++] = cur;
        }
        cur++;
    }
    return terms[n - 1];
}
