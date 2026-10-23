/*
 * A011734  A binary m-sequence: expansion of reciprocal of x^21 + x^2 + 1
 *          (mod 2, shifted by 20 initial 0's).
 * https://oeis.org/A011734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0..19)=0, a(20)=1; for n≥21: a(n)=a(n-21) XOR a(n-2)
 *            (recurrence derived from (x^21+x^2+1)·G(x)=x^20 over GF(2)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* holds a(0) … a(PERIOD-1)   */
static const idx_t PERIOD = 2097151; /* 2^21-1, the period of the m‑sequence */

seq_t a(idx_t n)
{
    /* initialise the period table on first call */
    if (!cache) {
        /* allocate memory for the entire period */
        cache = (seq_t *)malloc(PERIOD * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* fallback: should not happen */

        /* initial 20 zeros, then a(20)=1 */
        for (size_t i = 0; i < (size_t)20; ++i)
            cache[i] = (seq_t)0;
        cache[20] = (seq_t)1;

        /* compute the rest using the linear recurrence */
        for (size_t i = 21; i < (size_t)PERIOD; ++i) {
            /* a(i) = a(i-21) xor a(i-2)   (mod 2) */
            cache[i] = cache[i - 21] ^ cache[i - 2];
        }
    }

    /* reduce n modulo the period and return the stored value */
    size_t idx = (size_t)(n % PERIOD);
    return cache[idx];
}
