/*
 * A011705  A binary m-sequence: expansion of reciprocal of x^8+x^7+x^3+x^2+1.
 * https://oeis.org/A011705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8) XOR a(n-7) XOR a(n-3) XOR a(n-2) (mod 2) Initial
 *            values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=0,
 *            a(7)=1 The sequence is 255‑periodic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static bool _period_inited = false;
static seq_t _period[255];

/* initialise the 255‑term period once */
static void _init_period(void) {
    if (_period_inited) return;
    /* initial 8 terms */
    unsigned char buf[8] = {0,0,0,0,0,0,0,1};
    for (size_t i = 0; i < 8; ++i) _period[i] = (seq_t)buf[i];
    /* compute remaining terms */
    for (size_t i = 8; i < 255; ++i) {
        unsigned char new_bit =
            buf[(i-8)&7] ^ buf[(i-7)&7] ^ buf[(i-3)&7] ^ buf[(i-2)&7];
        buf[i & 7] = new_bit;
        _period[i] = (seq_t)new_bit;
    }
    _period_inited = true;
}

seq_t a(idx_t n) {
    _init_period();
    return _period[ n % 255 ];
}
