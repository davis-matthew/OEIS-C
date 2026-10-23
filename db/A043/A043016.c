/*
 * A043016  Base-7 palindromes that start with 2.
 * https://oeis.org/A043016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 114, u32 n <= 800, u64 n <= 800,
 *            u128 n <= 800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Generates the sequence A043016: base-7 palindromes that start with 2.
 * The terms are the decimal values of those palindromes.
 */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return 0;  /* offset is 1; return 0 for n=0 (undefined) */

    idx_t count = 0;          /* number of terms generated so far */

    /* maximum half length that will be needed for n up to 10000 */
    const unsigned MAX_HALF = 16;  /* safety buffer */
    uint8_t firstHalf[MAX_HALF];
    idx_t len, halfLen, combos;
    idx_t i, c, idx;

    for (len = 1; ; ++len) {
        halfLen = (len + 1) / 2;

        /* number of combinations for the free digits (excluding the fixed first digit 2) */
        combos = 1;
        for (i = 1; i < halfLen; ++i)
            combos *= 7;

        /* initialise the free digits to 0 */
        firstHalf[0] = 2;          /* leading digit fixed to 2 */
        for (i = 1; i < halfLen; ++i)
            firstHalf[i] = 0;

        for (c = 0; c < combos; ++c) {
            /* compute the decimal value of the current palindrome */
            seq_t val = 0;
            for (i = 0; i < len; ++i) {
                seq_t d = (i < halfLen) ?
                          (seq_t)firstHalf[i] :
                          (seq_t)firstHalf[len - 1 - i];
                val = val * 7 + d;
            }

            ++count;
            if (count == n)
                return val;

            /* increment the free digits (base-7 counter) for next combination */
            for (idx = halfLen - 1; idx > 0; --idx) {
                if (++firstHalf[idx] < 7)
                    break;          /* no carry, stop incrementing */
                firstHalf[idx] = 0; /* carry propagates */
            }
        }
    }
}
