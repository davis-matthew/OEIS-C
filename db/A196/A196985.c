/*
 * A196985  Number of nX4 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 2,3,4,1,0 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A196985
 *
 * Sequence definition:
 *   Number of n×4 arrays over {0,1,2,3,4} such that for each cell
 *   its value equals the number of its horizontal/vertical neighbors
 *   equal to 2,3,4,1,0 respectively for values 0,1,2,3,4.
 *
 * Recurrence (order 17, 1‑based index):
 *   a(n) = 2*a(n-1) + 6*a(n-2) + 25*a(n-3) + 33*a(n-4) + 1*a(n-5)
 *         -135*a(n-6) -117*a(n-7) +48*a(n-8) +141*a(n-9)
 *         -24*a(n-10) -135*a(n-11) -106*a(n-12)
 *         +18*a(n-13) +45*a(n-14) +27*a(n-15)
 *         +6*a(n-16)  +2*a(n-17)
 *
 * Initial values (n = 1 .. 17):
 *   1, 5, 25, 113, 547, 2499, 11647, 54845, 256395, 1198317,
 *   5607683, 26243839, 122783657, 574447047, 2687794239,
 *   12575875245, 58839924859
 */

#include <stddef.h>
#include <stdint.h>

/* Order of the recurrence */
#define ORDER 17

/* Initial terms a(1)..a(17) */
static const seq_t init_vals[ORDER] = {
    (seq_t)1ULL,
    (seq_t)5ULL,
    (seq_t)25ULL,
    (seq_t)113ULL,
    (seq_t)547ULL,
    (seq_t)2499ULL,
    (seq_t)11647ULL,
    (seq_t)54845ULL,
    (seq_t)256395ULL,
    (seq_t)1198317ULL,
    (seq_t)5607683ULL,
    (seq_t)26243839ULL,
    (seq_t)122783657ULL,
    (seq_t)574447047ULL,
    (seq_t)2687794239ULL,
    (seq_t)12575875245ULL,
    (seq_t)58839924859ULL
};

/*
 * Thread‑safe memoising is unnecessary; each call is independent.
 * The function works for any unsigned integer type seq_t and idx_t.
 */
seq_t a(idx_t n)
{
    if (n <= ORDER)
        return init_vals[n - 1];

    /* Buffer holds the last 17 computed values: buf[0] = a(k-1), buf[1] = a(k-2), … buf[16] = a(k-17) */
    seq_t buf[ORDER];

    /* initialise buffer with the known initial terms (reversed) */
    for (idx_t i = 0; i < ORDER; ++i)
        buf[i] = init_vals[ORDER - 1 - i];

    /* iterate from k = ORDER+1 up to n */
    idx_t k;
    for (k = ORDER + 1; k <= n; ++k) {
        /* compute next term using the recurrence */
        seq_t next =
            (seq_t)2 * buf[0] + (seq_t)6 * buf[1] + (seq_t)25 * buf[2] + (seq_t)33 * buf[3] + buf[4]
            - (seq_t)135 * buf[5] - (seq_t)117 * buf[6] + (seq_t)48 * buf[7] + (seq_t)141 * buf[8]
            - (seq_t)24 * buf[9] - (seq_t)135 * buf[10] - (seq_t)106 * buf[11]
            + (seq_t)18 * buf[12] + (seq_t)45 * buf[13] + (seq_t)27 * buf[14]
            + (seq_t)6 * buf[15] + (seq_t)2 * buf[16];

        /* shift buffer: drop the oldest term and insert the new one at position 0 */
        for (idx_t i = ORDER - 1; i > 0; --i)
            buf[i] = buf[i - 1];
        buf[0] = next;
    }

    /* after the loop, buf[0] holds a(n) */
    return buf[0];
}
