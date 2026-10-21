/*
 * A055984  a(n+1) = a(n) converted to base 10 from base 13.
 * https://oeis.org/A055984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 10; a(n+1) = interpret(a(n), base 13) → value of the
 *            decimal digits of a(n) read as a number in base 13
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 28, u64 n <= 34, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Computes a(n) for the OEIS sequence A055984.
 *   a(0) = 10
 *   a(n+1) = the base‑13 value of the decimal digits of a(n) */
seq_t a(idx_t n)
{
    seq_t cur = 10;              /* a(0) */

    if (n == 0)
        return cur;

    for (idx_t step = 1; step <= n; ++step) {
        /* 1. Extract decimal digits of cur into a buffer.          */
        seq_t tmp  = cur;
        size_t   len  = 0;            /* number of digits */
        size_t   cap  = 0;
        uint8_t *buf  = NULL;         /* buffer for digits          */

        /* Repeatedly get least‑significant decimal digit.         */
        while (tmp > (seq_t)0) {
            if (len == cap) {
                size_t newcap = (cap == 0) ? 64 : cap * 2;
                uint8_t *tmpbuf = (uint8_t *)realloc(buf, newcap);
                if (!tmpbuf) {             /* allocation failed */
                    free(buf);
                    return cur;          /* fallback, shouldn't happen */
                }
                buf = tmpbuf;
                cap = newcap;
            }
            buf[len++] = (uint8_t)(tmp % (seq_t)10);  /* digit 0–9 */
            tmp /= (seq_t)10;
        }

        if (len == 0) {                   /* only when cur == 0 */
            buf = (uint8_t *)realloc(buf, 1);
            buf[0] = 0;
            len = 1;
        }

        /* 2. Build the next value by interpreting the digits in
           base 13 (most significant digit first).                */
        seq_t next = 0;
        for (size_t k = 0; k < len; ++k) {
            next = next * (seq_t)13 + (seq_t)buf[len - 1 - k];
        }

        /* 3. Prepare for next iteration.                       */
        free(buf);
        cur = next;
    }

    return cur;
}
