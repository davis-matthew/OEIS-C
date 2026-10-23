/*
 * A036190  Log base 2 (n) mod 149.
 * https://oeis.org/A036190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 148, u16 n <= 148, u32 n <= 148, u64 n <= 148,
 *            u128 n <= 148, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036190 – discrete logarithm base 2 modulo 149.
   a(n) = the unique integer k with 0 ≤ k < 148 such that
   2^k ≡ n (mod 149); for n divisible by 149, a(n)=0.     */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* table[t] = exponent k with 2^k ≡ t (mod 149)   */
    static uint8_t table[149];
    static int init = 0;          /* has the table been filled?  */

    if (!init) {
        uint64_t v = 1;            /* current value of 2^k mod 149   */
        for (unsigned int k = 0; k < 148; ++k) {
            table[(int)v] = (uint8_t)k;
            v = (v * 2) % 149;
        }
        init = 1;
    }

    /* Reduce the argument modulo 149.  n ≥ 1, so the result is in 1…148. */
    idx_t r = n % (idx_t)149;

    /* If r == 0 (i.e., n is a multiple of 149) the definition takes a(n)=0. */
    return (seq_t)(r == 0 ? 0 : table[(int)r]);
}
