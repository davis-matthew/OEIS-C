/*
 * A105714  Smallest m such that 4 is at the n-th position of the decimal
 *          representation of the m-th Fibonacci number.
 * https://oeis.org/A105714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 6, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   num_digits0_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the smallest Fibonacci number whose n‑th decimal digit
 * (counting from the right, 0‑based) is '4'.  The function scans the
 * Fibonacci sequence from the start until it finds the first term that
 * meets the condition.  This strategy is fast enough for n up to about
 * 10 000 (the 10 001st Fibonacci number has ≈ 10 000 digits).  The
 * implementation uses only the utilities digit_len and seq_to_dec_string
 * provided by the framework. */
#include <stdlib.h>
#include <stddef.h>

#include <stdint.h>
#include <string.h>
#include "../utils/num_digits0_seq.h"

/* Convert a seq_t value to a decimal string.  The buffer must be
   large enough to hold the result.  */
static void seq_to_dec_string(seq_t val, char *buf, size_t buflen)
{
    seq_t v = val;
    if (v == 0) {
        if (buflen >= 2) {
            buf[0] = '0';
            buf[1] = '\0';
        }
        return;
    }
    size_t pos = 0;
    /* Extract digits in reverse order. */
    while (v != 0 && pos + 1 < buflen) {
        seq_t d = v % 10;
        v = v / 10;
        buf[pos++] = (char)('0' + (int)d);
    }
    /* Null‑terminate and reverse the string. */
    if (pos >= buflen) pos = buflen - 1;
    buf[pos] = '\0';
    for (size_t i = 0; i < pos / 2; ++i) {
        char tmp = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = tmp;
    }
}

seq_t a(idx_t n)
{
    /* Fibonacci numbers: F_0 = 0, F_1 = 1. */
    seq_t f_prev = 0;
    seq_t f_cur  = 1;            /* next Fibonacci number */

    /* Buffer reused for decimal conversion.  It grows only when needed. */
    static char *buf   = NULL;
    static size_t bufcap = 0;

    while (1) {
        /* Length of the current Fibonacci number in decimal. */
        size_t len = (size_t)num_digits0_seq(f_prev);

        if (len > n) {              /* the number has enough digits */
            size_t need = len + 1;  /* +1 for terminating '\0' */

            if (need > bufcap) {
                /* Reallocate buffer (C++ requires a cast from void*). */
                char *tmp = (char*)realloc(buf, need);
                if (!tmp) abort();
                buf = tmp;
                bufcap = need;
            }

            seq_to_dec_string(f_prev, buf, bufcap);

            /* Check the n‑th digit from the right (0‑based). */
            if (buf[len - 1 - n] == '4')
                return f_prev;
        }

        /* Advance to the next Fibonacci number. */
        seq_t f_next = f_prev + f_cur;
        f_prev = f_cur;
        f_cur  = f_next;
    }
}
