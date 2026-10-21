/*
 * A091416  Natural numbers written out and then regrouped into the smallest
 *          possible nontrivial blocks such that each begins and ends with the
 *          same digit. (Leading zeros omitted and not counted as beginning
 *          digit).
 * https://oeis.org/A091416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 3, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A091416 – concatenate natural numbers and split into smallest
    non‑trivial blocks that start and end with the same digit.
    The following implementation walks the infinite decimal stream
    until the requested block is found.  It uses only integer
    arithmetic and a small stack buffer, so it is O(total digits
    processed) and works for all builds.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 – for n == 0 return 0 for safety */
    if (n == 0)
        return 0;

    seq_t block_val = 0;               /* current block numeric value   */
    idx_t  collected = 0;              /* number of finished blocks     */

    uint64_t number   = 1;              /* current natural number        */
    char     buf[32];                  /* decimal digits of current number  */
    size_t   numlen    = 0;             /* length of buf                  */
    size_t   pos       = 0;             /* position in buf                */

    bool     in_block  = false;         /* we are inside a growing block  */
    char     start_digit = 0;           /* first digit of current block   */

    while (collected < n) {
        /* Load next digit from the current number if needed */
        if (pos == numlen) {
            /* Convert number to decimal string in buf */
            uint64_t tmp = number;
            char rev[32];
            size_t revlen = 0;
            if (tmp == 0) {            /* never happens here */
                rev[revlen++] = '0';
            } else {
                while (tmp > 0) {
                    rev[revlen++] = '0' + (tmp % 10);
                    tmp /= 10;
                }
            }
            for (size_t i = 0; i < revlen; ++i)
                buf[i] = rev[revlen-1-i];
            numlen = revlen;
            pos    = 0;
            ++number;                  /* next natural number          */
        }

        char d = buf[pos++];

        if (!in_block) {
            /* look for first non‑zero digit to start a block */
            if (d == '0')
                continue;              /* ignore leading zeros       */
            in_block     = true;
            start_digit  = d;
            block_val    = (seq_t)(d - '0');
        } else {
            block_val = block_val * (seq_t)10 + (seq_t)(d - '0');
            if (d == start_digit) {   /* block finished            */
                ++collected;
                if (collected == n)
                    return block_val;
                in_block = false;     /* next block starts with next digit */
            }
        }
    }

    /* unreachable – but required for completeness */
    return block_val;
}
