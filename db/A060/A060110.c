/*
 * A060110  Numbers in Morse code, with 1 for a dot, 2 for a dash and 0
 *          between digits/letters and then converted from base 3 to base 10.
 * https://oeis.org/A060110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = decimal value of the base‑3 number obtained by converting
 *            each decimal digit of n into its Morse code representation with
 *            dot->1, dash->2, inserting a 0 between successive digits
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Morse code for decimal digits 0–9, using 1 for dot and 2 for dash */
    static const char *morse[10] = {
        "22222", /* 0: -----   */
        "12222", /* 1: .----   */
        "11222", /* 2: ..---   */
        "11122", /* 3: ...--   */
        "11112", /* 4: ....-   */
        "11111", /* 5: .....   */
        "21111", /* 6: -....   */
        "22111", /* 7: --...   */
        "22211", /* 8: ---..   */
        "22221"  /* 9: ----.   */
    };

    /* Extract decimal digits of n (least significant first) */
    int digits[20];
    int cnt = 0;
    if (n == 0) {
        digits[cnt++] = 0;
    } else {
        idx_t tmp = n;
        while (tmp) {
            digits[cnt++] = tmp % 10;
            tmp /= 10;
        }
    }

    /* Build the base‑3 representation and convert to decimal */
    seq_t value = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        const char *pat = morse[digits[i]];
        /* 5 symbols per Morse code digit */
        for (int k = 0; k < 5; ++k) {
            value = value * 3 + (pat[k] - '0');
        }
        /* Separator 0 between digits, except after the last one */
        if (i > 0) {
            value = value * 3;
        }
    }
    return value;
}
