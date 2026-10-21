/*
 * A221230  Number of nX2 arrays of occupancy after each element moves to some
 *          horizontal, vertical or antidiagonal neighbor, with no occupancy
 *          greater than 2.
 * https://oeis.org/A221230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) -48*a(n-2) +60*a(n-3) -124*a(n-4) +370*a(n-5)
 *            -458*a(n-6) +370*a(n-7) -768*a(n-8) +889*a(n-9) -476*a(n-10)
 *            +126*a(n-11) -17*a(n-12) +a(n-13) with initial values: a(1)=1,
 *            a(2)=14, a(3)=119, a(4)=852, a(5)=6360, a(6)=47334, a(7)=351402,
 *            a(8)=2609499, a(9)=19373481, a(10)=143810755,...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices lower than the defined offset */
    if (n < 1)
        return (seq_t)0;

    /* Initial values for n = 1 .. 13 */
    static const seq_t init[14] = {
        0ull,               /* padding for 0 index */
        1ull, 14ull, 119ull, 852ull, 6360ull, 47334ull,
        351402ull, 2609499ull, 19373481ull, 143810755ull,
        1067445448ull, 7922843437ull, 58803820979ull
    };

    if (n <= 13)
        return init[(size_t)n];

    /* Use a circular buffer of size 14 (13 previous terms + the current one).
       ring[k] holds a(k+1) for k = 0 .. 13;  ring[(k-1)%14] = a(k).   */
    seq_t ring[14];

    /* Initialise the buffer with the known values a(1) .. a(13) */
    for (size_t i = 1; i <= 13; ++i)
        ring[i % 14] = init[i];

    /* Compute terms a(14) .. a(n) */
    for (size_t k = 14; k <= (size_t)n; ++k) {
        /* Each coefficient times the appropriate lagged term.  Use
           modulo 14 indexing: ring[(k-j)%14] = a(k-j) for j ≥ 1.      */
        seq_t pos = ring[(k - 1) % 14] * 13
                  + ring[(k - 3) % 14] * 60
                  + ring[(k - 5) % 14] * 370
                  + ring[(k - 7) % 14] * 370
                  + ring[(k - 9) % 14] * 889
                  + ring[(k -11) % 14] * 126
                  + ring[(k -13) % 14] * 1;

        seq_t neg = ring[(k - 2) % 14] * 48
                  + ring[(k - 4) % 14] * 124
                  + ring[(k - 6) % 14] * 458
                  + ring[(k - 8) % 14] * 768
                  + ring[(k -10) % 14] * 476
                  + ring[(k -12) % 14] * 17;

        ring[k % 14] = pos - neg;
    }

    return ring[n % 14];
}
