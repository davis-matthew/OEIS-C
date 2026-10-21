/*
 * A223320  Rolling icosahedron footprints: number of nX7 0..11 arrays
 *          starting with 0 where 0..11 label vertices of an icosahedron and
 *          every array movement to a horizontal or antidiagonal neighbor
 *          moves along an icosahedral edge.
 * https://oeis.org/A223320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4313*a(n-1) - 4293540*a(n-2) + 1835925129*a(n-3) -
 *            400542201531*a(n-4) + 46506128233194*a(n-5) -
 *            2762797384141236*a(n-6) + 72983536818080616*a(n-7) -
 *            643864107456947520*a(n-8) for n > 8 initial values: a(1)=15625,
 *            a(2)=46411625, a(3)=142980696625, a(4)=443803619416625,
 *            a(5)=137946414496346...
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n) {
    /* Lazily initialized initial values as arbitrary precision strings */
    static seq_t init[8];
    static bool init_done = false;
    if (!init_done) {
        init[0] = seq_from_str("15625");
        init[1] = seq_from_str("46411625");
        init[2] = seq_from_str("142980696625");
        init[3] = seq_from_str("443803619416625");
        init[4] = seq_from_str("1379464144963464625");
        init[5] = seq_from_str("4288761014162797342625");
        init[6] = seq_from_str("13334277936752000032650625");
        init[7] = seq_from_str("41458126974532997739670258625");
        init_done = true;
    }

    /* OEIS offset is 1 */
    if (n < 1) return 0;

    if (n <= 8) return init[n-1];

    /* Buffer to hold the last 8 terms */
    seq_t buf[8];
    for (size_t k = 0; k < 8; ++k) buf[k] = init[k];

    for (idx_t i = 9; i <= n; ++i) {
        seq_t pos = (seq_t)4313ULL * buf[7]
                  + (seq_t)1835925129ULL * buf[5]
                  + (seq_t)46506128233194ULL * buf[3]
                  + (seq_t)72983536818080616ULL * buf[1];

        seq_t neg = (seq_t)4293540ULL * buf[6]
                  + (seq_t)400542201531ULL * buf[4]
                  + (seq_t)2762797384141236ULL * buf[2]
                  + (seq_t)643864107456947520ULL * buf[0];

        seq_t t = pos - neg; /* t is guaranteed non‑negative */

        /* Shift buffer left by one position */
        for (size_t k = 0; k < 7; ++k) buf[k] = buf[k+1];
        buf[7] = t;
    }

    return buf[7];
}
