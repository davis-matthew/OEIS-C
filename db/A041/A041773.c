/*
 * A041773  Denominators of continued fraction convergents to sqrt(407).
 * https://oeis.org/A041773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041773: Denominators of continued fraction convergents to sqrt(407).
   Recurrence: a(n) = 5326*a(n-6) - a(n-12)  for n > 11.
   Initial values:
     a(0)=1, a(1)=5, a(2)=6, a(3)=17, a(4)=23, a(5)=132,
     a(6)=5303, a(7)=26647, a(8)=31950, a(9)=90547,
     a(10)=122497, a(11)=703032. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Circular buffer holding the last 12 computed terms. */
    static seq_t buf[12];
    /* Predefined initial 12 terms. */
    static const seq_t init[12] = {
        (seq_t)1, (seq_t)5, (seq_t)6, (seq_t)17, (seq_t)23, (seq_t)132,
        (seq_t)5303, (seq_t)26647, (seq_t)31950, (seq_t)90547,
        (seq_t)122497, (seq_t)703032
    };
    static bool initialized = false;
    /* Index of the highest n for which buf[n % 12] holds the correct value. */
    static idx_t last_computed = 0;

    if (!initialized) {
        for (idx_t i = 0; i < 12; ++i)
            buf[i] = init[i];
        last_computed = 11;          /* a(0)…a(11) are now available */
        initialized = true;
    }

    if (n <= last_computed)
        return buf[n % 12];

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* a(i) = 5326*a(i-6) - a(i-12) */
        buf[i % 12] = (seq_t)5326 * buf[(i - 6) % 12] - buf[(i - 12) % 12];
    }

    last_computed = n;
    return buf[n % 12];
}
