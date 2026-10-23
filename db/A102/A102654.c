/*
 * A102654  Add 7 to each of the preceding digits, beginning with 0.
 * https://oeis.org/A102654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = digit-wise expansion of a(n-1) by adding 7 to each digit
 *            and generating the sequence breadth‑first
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static storage for sequence terms */
    static seq_t *terms = NULL;          /* dynamic array of sequence values */
    static idx_t size = 0;               /* number of terms stored */
    static idx_t capacity = 0;           /* allocated capacity of terms */
    static idx_t next_expand = 0;        /* index of next term to expand */

    /* initialize storage on first call */
    if (terms == NULL) {
        capacity = 64;
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (terms == NULL) return (seq_t)0;   /* allocation failed, return 0 */
        terms[0] = (seq_t)0;                  /* a(0) */
        size = 1;
        next_expand = 0;
    }

    /* generate terms up to index n */
    while (size <= n) {
        seq_t cur = terms[next_expand];

        /* extract decimal digits of cur into array (least significant first) */
        uint8_t digits[64];       /* maximum needed for 128‑bit numbers */
        int nd = 0;

        if (cur == (seq_t)0) {
            digits[nd++] = 0;
        } else {
            seq_t tmp = cur;
            while (tmp > (seq_t)0) {
                digits[nd++] = (uint8_t)(tmp % (seq_t)10);
                tmp /= (seq_t)10;
            }
        }

        /* produce new terms from digits (left to right) */
        for (int i = nd - 1; i >= 0; --i) {
            seq_t child = (seq_t)digits[i] + (seq_t)7;

            if (size == capacity) {
                capacity = (capacity == 0) ? 64 : capacity * 2;
                terms = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
                if (terms == NULL) return (seq_t)0;   /* allocation failed */
            }
            terms[size++] = child;
        }

        ++next_expand;
    }

    return terms[n];
}
