/*
 * A091425  Write the natural numbers in a triangle with k digits in the k-th
 *          row, as shown below. Sequence gives the leading diagonal.
 * https://oeis.org/A091425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the digit at position n*(n+1)/2 in the infinite decimal
 *            string formed by writing the natural numbers 1,2,3,...
 *            consecutively. The leading diagonal of the triangular array with
 *            k digits in row k consists of the last digit of each row, i.e.
 *            the digit at the cumulative length S(k)=k*(k+1)/2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* position in the digit stream (1‑based) */
    size_t pos = (size_t)((unsigned __int128)n * (n + 1) / 2);

    static unsigned char *buf = NULL;    /* cached digit stream */
    static size_t buflen = 0;            /* number of digits stored */
    static size_t bufcap = 0;            /* allocated capacity of buf */
    static unsigned long long next_num = 1;  /* next natural number to convert */

    /* Append digits until we cover the requested position */
    while (buflen < pos) {
        unsigned long long num = next_num;
        /* convert current number to decimal digits in tmp */
        char tmp[20];
        int tlen = 0;
        if (num == 0) {
            tmp[tlen++] = '0';
        } else {
            while (num > 0) {
                tmp[tlen++] = (char)('0' + (num % 10));
                num /= 10;
            }
            /* reverse to correct order */
            for (int i = 0; i < tlen / 2; ++i) {
                char c = tmp[i];
                tmp[i] = tmp[tlen - 1 - i];
                tmp[tlen - 1 - i] = c;
            }
        }

        /* ensure enough space in buf */
        if (buflen + (size_t)tlen > bufcap) {
            size_t newcap = bufcap ? bufcap * 2 : 1024;
            while (newcap < buflen + (size_t)tlen) newcap <<= 1;
            unsigned char *nbuf = (unsigned char *)realloc(buf, newcap);
            buf = nbuf;
            bufcap = newcap;
        }

        /* copy digits into the buffer */
        for (int i = 0; i < tlen; ++i) {
            buf[buflen++] = (unsigned char)(tmp[i] - '0');
        }
        ++next_num;
    }

    /* the desired digit is at index pos-1 (0‑based) */
    return (seq_t)buf[pos - 1];
}
