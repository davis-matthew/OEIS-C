/*
 * A011706  A binary m-sequence: expansion of reciprocal of x^8+x^6+x^5+x^2+1.
 * https://oeis.org/A011706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A011706: binary m‑sequence (reciprocal of x^8+x^6+x^5+x^2+1).
 * The bits satisfy the linear recurrence in GF(2):
 *   a(n) = a(n-8) + a(n-6) + a(n-5) + a(n-2) (mod 2)
 * with initial terms a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0,
 * a(5)=0, a(6)=0, a(7)=1. The sequence is 255‑periodic.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static table of one full period (255 bits) */
    static seq_t period[255];
    static bool initialized = false;

    if (!initialized) {
        /* first 8 bits from the definition */
        period[0] = 0;
        period[1] = 0;
        period[2] = 0;
        period[3] = 0;
        period[4] = 0;
        period[5] = 0;
        period[6] = 0;
        period[7] = 1;

        /* compute the rest of the period using the recurrence */
        for (idx_t i = 8; i < 255; ++i) {
            /* addition in GF(2) is XOR of the terms */
            period[i] = period[i-8] ^ period[i-6] ^ period[i-5] ^ period[i-2];
        }
        initialized = true;
    }

    /* return the term at index n, modulo the period length */
    return period[n % 255];
}
