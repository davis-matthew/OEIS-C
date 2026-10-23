/*
 * A102258  Begin with 5, multiply each digit by 2.
 * https://oeis.org/A102258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements generation by doubling each decimal digit:
   a(1) = 5; for n > 1, a(n) is produced in order by multiplying
   every digit of each preceding term by 2. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range; offset is 1 */

    /* Rough upper bound: each term generates at most two new terms. */
    size_t capacity = 2 * (size_t)n + 16;
    seq_t *arr = (seq_t *)malloc(capacity * sizeof(seq_t));
    if (!arr) return 0;             /* allocation failure */

    size_t  len = 1;                /* number of elements stored */
    arr[0] = 5;                     /* a(1) */
    size_t  read = 0;               /* next element to process */

    while (len <= (size_t)n) {
        seq_t cur = arr[read++];
        if (cur == 0) {             /* single zero digit */
            if (len == capacity) {
                capacity *= 2;
                arr = (seq_t *)realloc(arr, capacity * sizeof(seq_t));
                if (!arr) return 0;
            }
            arr[len++] = (seq_t)0;  /* 0*2 */
            continue;
        }

        /* Extract decimal digits of cur from most significant to least. */
        seq_t temp = cur;
        int digits[32];            /* enough for any realistic value */
        int dcnt = 0;
        while (temp > 0) {
            digits[dcnt++] = (int)(temp % 10);
            temp /= 10;
        }

        for (int i = dcnt - 1; i >= 0; --i) {
            if (len == capacity) {
                capacity *= 2;
                arr = (seq_t *)realloc(arr, capacity * sizeof(seq_t));
                if (!arr) return 0;
            }
            arr[len++] = (seq_t)(digits[i] * 2);
        }
    }

    seq_t result = arr[n - 1];
    free(arr);
    return result;
}
