/*
 * A084043  Smallest palindromic multiple of n in which the digit string of n
 *          appears as sandwiched between at least a pair of digits, or 0 if n
 *          = 10k or no such number exists.
 * https://oeis.org/A084043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for the smallest palindrome multiple of
   n that contains the decimal representation of n sandwiched between at
   least one digit on each side.  Returns 0 when n is a multiple of 10. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* Convert an arbitrary unsigned seq_t value to a null‑terminated
   decimal string.  The length of the number (without the terminator)
   is stored in *len.  Works for uint8_t…uint64_t, unsigned __int128,
   and arbitrary‑precision types that support the standard arithmetic
   operators. */
static void seq_to_dec_str(seq_t val, char *buf, size_t *len)
{
    if (val == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        *len = 1;
        return;
    }
    char tmp[200];
    size_t pos = 0;
    while (val > 0) {
        uint8_t digit = (uint8_t)(val % 10);
        tmp[pos++] = '0' + digit;
        val /= 10;
    }
    for (size_t i = 0; i < pos; ++i) {
        buf[i] = tmp[pos - 1 - i];
    }
    buf[pos] = '\0';
    *len = pos;
}

seq_t a(idx_t n)
{
    /* 0 for n that end with 0 (conjecture: no answer). */
    if ((n % 10) == 0) {
        return 0;
    }

    /* Convert n to string once. */
    char nbuf[200];
    size_t nlen;
    seq_to_dec_str((seq_t)n, nbuf, &nlen);

    for (idx_t k = 1; ; ++k) {
        seq_t m = (seq_t)n * (seq_t)k;

        /* Convert m to string. */
        char mbuf[200];
        size_t mlen;
        seq_to_dec_str(m, mbuf, &mlen);

        /* Must have at least one digit before and after n. */
        if (mlen < nlen + 2) {
            continue;
        }

        /* Palindrome check. */
        bool pal = true;
        for (size_t i = 0, j = mlen - 1; i < j; ++i, --j) {
            if (mbuf[i] != mbuf[j]) {
                pal = false;
                break;
            }
        }
        if (!pal) {
            continue;
        }

        /* Search for nstr inside mstr ensuring it is sandwiched. */
        for (char *p = mbuf; (p = strstr(p, nbuf)); ++p) {
            size_t idx = (size_t)(p - mbuf);
            if (idx > 0 && (idx + nlen) < mlen) {
                return m;
            }
        }
    }
}
