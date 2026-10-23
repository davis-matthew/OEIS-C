/*
 * A188561  Number of nX3 binary arrays without the pattern 1 1 1 diagonally,
 *          vertically or horizontally.
 * https://oeis.org/A188561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) for n >= 1 is defined by the order‑15 linear recurrence
 *            a(n) = a(n-1) + 11*a(n-2) + 60*a(n-3) + 89*a(n-4) + 107*a(n-5) -
 *            75*a(n-6) - 50*a(n-7) - 157*a(n-8) + 184*a(n-9) - 91*a(n-10) +
 *            86*a(n-11) - 35*a(n-12) + 13*a(n-13) - 14*a(n-14) + 4*a(n-15)
 *            with initial values a(1)=7 a(2)=49 a(3)=247 a(...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial 15 terms */
    static const seq_t init[15] = {
        (seq_t)7ull,
        (seq_t)49ull,
        (seq_t)247ull,
        (seq_t)1383ull,
        (seq_t)7722ull,
        (seq_t)42712ull,
        (seq_t)237116ull,
        (seq_t)1315783ull,
        (seq_t)7300042ull,
        (seq_t)40505756ull,
        (seq_t)224748056ull,
        (seq_t)1247024929ull,
        (seq_t)6919195733ull,
        (seq_t)38391539384ull,
        (seq_t)213017628693ull
    };

    if (n <= 15)
        return init[n - 1];

    /* Buffer holding the last 15 terms: buf[0] = a(k-15), buf[14] = a(k-1) */
    seq_t buf[15];
    for (int i = 0; i < 15; ++i)
        buf[i] = init[i];

    /* Compute a(k) for k = 16 .. n */
    for (idx_t k = 16; k <= n; ++k) {
        /* Index in buf corresponding to a(k-1) */
        int idx = (int)((k - 2) % 15);

        seq_t res = 0;
        res += buf[idx];
        res += 11 * buf[(idx - 1 + 15) % 15];
        res += 60 * buf[(idx - 2 + 15) % 15];
        res += 89 * buf[(idx - 3 + 15) % 15];
        res += 107 * buf[(idx - 4 + 15) % 15];
        res -= 75 * buf[(idx - 5 + 15) % 15];
        res -= 50 * buf[(idx - 6 + 15) % 15];
        res -= 157 * buf[(idx - 7 + 15) % 15];
        res += 184 * buf[(idx - 8 + 15) % 15];
        res -= 91 * buf[(idx - 9 + 15) % 15];
        res += 86 * buf[(idx - 10 + 15) % 15];
        res -= 35 * buf[(idx - 11 + 15) % 15];
        res += 13 * buf[(idx - 12 + 15) % 15];
        res -= 14 * buf[(idx - 13 + 15) % 15];
        res += 4 * buf[(idx - 14 + 15) % 15];

        /* Store a(k) in buffer for future steps */
        buf[(k - 1) % 15] = res;
    }

    return buf[(n - 1) % 15];
}
