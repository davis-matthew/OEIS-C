/*
 * A174200  The sequence of numbers which, when entered on a standard
 *          telephone numeric keypad, produce the letters a thru z. Letters
 *          are accessed by one or more presses of a single key.
 * https://oeis.org/A174200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = key(n) repeated r(n) times (telephone keypad mapping)
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t idx = n - 1;          /* 0‑based index of the letter */

    int key;                    /* digit to use on the keypad */
    int rep;                    /* number of repeats of that digit */

    if (idx < 15) {
        /* Keys 2–6, each with 3 letters */
        key = 2 + idx / 3;
        rep = idx % 3 + 1;
    } else if (idx < 19) {
        /* Key 7 (4 letters) */
        key = 7;
        rep = idx - 15 + 1;
    } else if (idx < 22) {
        /* Key 8 (3 letters) */
        key = 8;
        rep = idx - 19 + 1;
    } else if (idx < 26) {
        /* Key 9 (4 letters) */
        key = 9;
        rep = idx - 22 + 1;
    } else {
        /* Undefined for n > 26 (not part of the sequence) */
        return (seq_t)0;
    }

    /* Construct the number consisting of `rep` copies of `key` */
    seq_t val = 0;
    for (int i = 0; i < rep; ++i) {
        val = val * 10 + key;
    }

    return val;
}
