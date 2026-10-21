/*
 * A093086  "Fibonacci in digits": start with a(0)=0, a(1)=1; repeatedly
 *          adjoin the digits of the sum of the next two terms.
 * https://oeis.org/A093086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8532, u16 n <= 8532, u32 n <= 8532, u64 n <= 8532,
 *            u128 n <= 8532, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) is produced by repeatedly appending the decimal
 * digits of the sum of the two preceding terms.
 * Initial terms: a(0)=0, a(1)=1.
 * This algorithm builds the sequence lazily, extending a static
 * buffer up to the requested index n. */

#include <stdlib.h>   /* malloc, free */
#include <string.h>   /* memcpy */

static uint8_t *seq_buf = NULL;   /* buffer holding the digits */
static size_t   seq_size = 0;     /* number of digits stored so far */
static size_t   seq_cap  = 0;     /* current allocation size      */
static idx_t    next_j   = 0;     /* index of the next pair to sum */

static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    uint8_t *new_buf = (uint8_t*)malloc(new_cap);
    if (seq_buf) {
        memcpy(new_buf, seq_buf, seq_size);
        free(seq_buf);
    }
    seq_buf = new_buf;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* lazily initialise the buffer with the first two terms            */
    if (!seq_buf) {
        seq_cap = 64;
        seq_buf = (uint8_t*)malloc(seq_cap);
        seq_buf[0] = 0;
        seq_buf[1] = 1;
        seq_size = 2;
        next_j    = 0;
    }

    /* extend the sequence until it contains at least n+1 digits       */
    while (seq_size <= (size_t)n) {
        /* compute the sum of two current digits                          */
        seq_t sum = (seq_t)seq_buf[next_j] + (seq_t)seq_buf[next_j + 1];

        /* append the digits of the sum (at most two digits)             */
        if (sum < 10) {
            ensure_capacity(seq_size + 1);
            seq_buf[seq_size++] = (uint8_t)sum;
        } else {   /* sum is 10 .. 18 */
            ensure_capacity(seq_size + 2);
            seq_buf[seq_size++] = (uint8_t)(sum / 10);  /* most significant */
            seq_buf[seq_size++] = (uint8_t)(sum % 10);  /* least significant */
        }
        next_j++;   /* advance to the next pair to be summed         */
    }

    return (seq_t)seq_buf[(size_t)n];
}
