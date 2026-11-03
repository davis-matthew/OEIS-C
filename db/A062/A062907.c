/*
 * A062907  Numbers k such that k and its reversal are both multiples of 19.
 * https://oeis.org/A062907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 185, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 968 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062907:
 * Numbers k such that k and its decimal reversal are both multiples of 19.
 * a(n) is the nth smallest such number, starting with a(1)=0. */
#include <stdbool.h>

static inline bool rev_mult19(seq_t k)
{
    /* Return true iff reverse of k is divisible by 19. */
    int revMod = 0;          /* remainder mod 19 of the reversed number */
    seq_t x = k;
    /* Handle 0 separately – its reverse is 0, which is a multiple of 19. */
    if (x == 0)
        return true;
    while (x != (seq_t)0) {
        int d = (int)(x % (seq_t)10);
        revMod = (revMod * 10 + d) % 19;
        x /= (seq_t)10;
    }
    return revMod == 0;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t k = 0;; k += (seq_t)19) {
        if (rev_mult19(k)) {
            ++count;
            if (count == n)
                return k;
        }
    }
}
