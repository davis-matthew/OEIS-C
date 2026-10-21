/*
 * A063460  A Beatty sequence: a(n) = floor(n * (Pi-1)/(Pi-2)).
 * https://oeis.org/A063460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor( n * 10^30 / 1141592653589793238462643383279 )
 * Range:     u8 n <= 136, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   This is equivalent to floor( n * (Pi-1)/(Pi-2) ) since (Pi-1)/(Pi-2) = 1 + 1/(Pi-2)  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 10^30 as an unsigned 128‑bit integer */
    static unsigned __int128 NUM = 0;
    /* Pi-2 expressed as 1141592653589793238462643383279 / 10^30  */
    static unsigned __int128 DEN = 0;

    if (!NUM) {
        NUM = 1;
        for (int i = 0; i < 30; ++i)
            NUM *= 10;
    }

    if (!DEN) {
        const char *s = "1141592653589793238462643383279";
        for (const char *p = s; *p; ++p) {
            DEN = DEN * 10 + (unsigned __int128)(*p - '0');
        }
    }

    unsigned __int128 prod = (unsigned __int128)n * NUM;
    unsigned __int128 inc  = prod / DEN;          /* floor(n * NUM / DEN)  */
    seq_t result = (seq_t)n + (seq_t)inc;
    return result;
}
