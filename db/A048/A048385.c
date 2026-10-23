/*
 * A048385  In base-10 notation replace digits of n with their squared values
 *          (Version 1).
 * https://oeis.org/A048385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate the squares of each decimal digit of n
 * Range:     u8 n <= 23, u16 n <= 343, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n)
{
    /* Special case for n == 0 */
    if (n == 0)
        return 0;

    /* Helper tables: square of each digit and its decimal length */
    static const seq_t digit_sq[10] = {0,1,4,9,16,25,36,49,64,81};
    static const unsigned char digit_len[10] = {1,1,1,1,2,2,2,2,2,2};

    /* Temporary buffers to hold squares in reverse order (least significant digit first) */
    uint16_t sq_buf[40];
    unsigned char len_buf[40];
    int pos = 0;

    /* Extract digits from n (least significant first) and store their squares */
    idx_t tmp = n;
    while (tmp != 0) {
        unsigned d = tmp % 10;
        tmp /= 10;
        sq_buf[pos] = (uint16_t)digit_sq[d];
        len_buf[pos] = digit_len[d];
        pos++;
    }

    /* Build the result by processing the squares from most significant to least */
    seq_t res = 0;
    for (int i = pos - 1; i >= 0; --i) {
        if (len_buf[i] == 1) {
            res = res * 10 + sq_buf[i];
        } else {               /* length 2 */
            res = res * 100 + sq_buf[i];
        }
    }
    return res;
}
