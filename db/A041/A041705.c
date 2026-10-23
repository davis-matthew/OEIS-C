/*
 * A041705  Denominators of continued fraction convergents to sqrt(372).
 * https://oeis.org/A041705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 24302*a(n-6) - a(n-12) (n > 11) Initial values (n = 0 ..
 *            11): 1, 3, 7, 87, 181, 630, 24121, 72993, 170107, 2114277,
 *            4398661, 15310260
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 14, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

extern seq_t a(idx_t n)
{
    /* base case: first 12 elements */
    if (n < 12) {
        static const seq_t init[12] = {
            (seq_t)1ULL,
            (seq_t)3ULL,
            (seq_t)7ULL,
            (seq_t)87ULL,
            (seq_t)181ULL,
            (seq_t)630ULL,
            (seq_t)24121ULL,
            (seq_t)72993ULL,
            (seq_t)170107ULL,
            (seq_t)2114277ULL,
            (seq_t)4398661ULL,
            (seq_t)15310260ULL
        };
        return init[(size_t)n];
    }

    /* compute by recurrence, storing only the last 12 values */
    static seq_t buf[12];
    for (size_t i = 0; i < 12; ++i) {
        buf[i] = (seq_t) ( (idx_t)i < 12 ? 
            ( (i==0)?1ULL:(i==1)?3ULL:(i==2)?7ULL:(i==3)?87ULL:
              (i==4)?181ULL:(i==5)?630ULL:(i==6)?24121ULL:
              (i==7)?72993ULL:(i==8)?170107ULL:
              (i==9)?2114277ULL:(i==10)?4398661ULL:15310260ULL ) 
            : 0 );  /* This branch is never taken */
    }

    /* Actually initialise with the known first twelve terms */
    buf[0]  = (seq_t)1ULL;
    buf[1]  = (seq_t)3ULL;
    buf[2]  = (seq_t)7ULL;
    buf[3]  = (seq_t)87ULL;
    buf[4]  = (seq_t)181ULL;
    buf[5]  = (seq_t)630ULL;
    buf[6]  = (seq_t)24121ULL;
    buf[7]  = (seq_t)72993ULL;
    buf[8]  = (seq_t)170107ULL;
    buf[9]  = (seq_t)2114277ULL;
    buf[10] = (seq_t)4398661ULL;
    buf[11] = (seq_t)15310260ULL;

    /* iterate up to n */
    for (idx_t i = 12; i <= n; ++i) {
        size_t idx = (size_t)i % 12;
        size_t i6  = (size_t)(i - 6) % 12;
        size_t i12 = (size_t)(i - 12) % 12;
        buf[idx] = (seq_t)24302 * buf[i6] - buf[i12];
    }

    return buf[(size_t)n % 12];
}
