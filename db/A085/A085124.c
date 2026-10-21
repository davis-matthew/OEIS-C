/*
 * A085124  Smallest multiple of n whose digital product is also a positive
 *          multiple of n, or 0 if no such number exists.
 * https://oeis.org/A085124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 195, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for the smallest multiple of n whose
   digit product is a positive multiple of n.  Returns 0 if n is not
   a 7‑smooth number or is a multiple of 10.  The search is performed
   by testing successive multiples m = n·k until the conditions hold. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n must not be divisible by 10 */
    if ((n % 10) == 0) return 0;

    /* n must be 7‑smooth: only prime factors 2,3,5 or 7 */
    idx_t t = n;
    while (t % 2 == 0) t /= 2;
    while (t % 3 == 0) t /= 3;
    while (t % 5 == 0) t /= 5;
    while (t % 7 == 0) t /= 7;
    if (t != 1) return 0;

    /* Search successive multiples m = n·k */
    for (idx_t k = 1; ; ++k) {
        seq_t m   = (seq_t)n * (seq_t)k;  /* current multiple           */
        seq_t tmp = m;                    /* copy for digit extraction   */
        seq_t prod = 1;                   /* product of digits modulo n */
        bool zero_digit = false;

        /* Extract digits and compute product modulo n */
        while (tmp > 0) {
            int d = (int)(tmp % 10);
            if (d == 0) { zero_digit = true; break; }
            prod = (prod * (seq_t)d) % (seq_t)n;
            tmp  = tmp / 10;
        }

        /* If no zero digit and product ≡ 0 (mod n), m is the answer */
        if (!zero_digit && prod == 0) return m;
        /* Otherwise continue searching */
    }
}
