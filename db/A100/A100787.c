/*
 * A100787  First differences give the same sequence written as a string of
 *          individual digits.
 * https://oeis.org/A100787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = a(n‑1) + digit_{n‑1}, where digit_k
 *            is the k‑th decimal digit of the concatenation of a(1)…a(k). The
 *            first difference sequence equals the sequence's own digits
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) is defined as 1 */
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    if (n == 1) return (seq_t)1;

    /* dynamic array to store the decimal digits of the union string so far */
    size_t cap = 256;                     /* initial capacity */
    uint8_t *digits = (uint8_t *)malloc(cap);
    if (digits == NULL) return (seq_t)1;   /* fallback if allocation fails */

    size_t len = 0;                       /* number of digits stored */
    digits[len++] = 1;                    /* digit of a(1) */

    seq_t prev = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i) {
        /* difference is the (i‑2)-th digit (zero‑based) of the string */
        seq_t diff = (seq_t)digits[i - 2];
        seq_t curr = prev + diff;

        /* append decimal digits of curr to the array */
        seq_t temp = curr;
        uint8_t rev[32];
        size_t cnt = 0;
        if (temp == (seq_t)0) {
            rev[cnt++] = 0;
        } else {
            while (temp != (seq_t)0) {
                rev[cnt++] = (uint8_t)(temp % (seq_t)10);
                temp = temp / (seq_t)10;
            }
        }
        /* copy in correct order */
        for (size_t j = 0; j < cnt; ++j) {
            if (len == cap) {
                cap <<= 1;                     /* double */
                uint8_t *newptr = (uint8_t *)realloc(digits, cap);
                if (newptr == NULL) {           /* allocation failed */
                    free(digits);
                    return curr;               /* return what we have */
                }
                digits = newptr;
            }
            digits[len++] = rev[cnt - 1 - j];
        }

        prev = curr;
    }

    free(digits);
    return prev;
}
