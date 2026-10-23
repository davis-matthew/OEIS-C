/*
 * A011730  A binary m-sequence: expansion of reciprocal of x^17+x^3+1.
 * https://oeis.org/A011730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-17) XOR a(n-3) with a(0)=…=a(15)=0, a(16)=1, periodic
 *            with period 131071 (=2^17-1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* period of the sequence */
    const idx_t PERIOD = 131071ULL;

    /* static cache of one full period */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache == NULL) {
        /* allocate space for the period */
        cache = (seq_t*)malloc(PERIOD * sizeof(seq_t));
        if (cache == NULL) {
            return (seq_t)0;   /* unlikely, but keeps the compiler happy */
        }

        /* initialise the first 17 terms */
        for (idx_t i = 0; i <= 16; ++i) {
            cache[i] = (i == 16) ? (seq_t)1 : (seq_t)0;
        }

        /* generate the remaining terms up to PERIOD-1 */
        for (idx_t i = 17; i < PERIOD; ++i) {
            cache[i] = cache[i-3] ^ cache[i-17];
        }

        cache_size = PERIOD;
    }

    /* reduce the index modulo the period */
    idx_t m = n % PERIOD;
    return cache[m];
}
