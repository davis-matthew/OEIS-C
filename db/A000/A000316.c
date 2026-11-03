/*
 * A000316  Two decks each have n kinds of cards, 2 of each kind. The first
 *          deck is laid out in order. The second deck is shuffled and laid
 *          out next to the first. A match occurs if a card from the second
 *          deck is next to a card of the same kind from the first deck. a(n)
 *          is the number of ways of achieving no m...
 * https://oeis.org/A000316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   (2*n-3)*a(n) = 2*(n-1)*(2*n-1)^2*a(n-1) + 4*(n-1)*(2*n-3)*a(n-2)
 *            - 16*(n-2)*(n-1)*(2*n-1)*a(n-3), a(0) = 1, a(1) = 0, a(2) = 4
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

/* Arbitrary precision: exact recurrence; the right-hand side is positive, so add before subtracting. */
static seq_t a316_big(idx_t n)
{
    seq_t x0 = 1, x1 = 0, x2 = 4;
    idx_t k;
    if (n == 0) return x0;
    if (n == 1) return x1;
    if (n == 2) return x2;
    for (k = 3; k <= n; k++) {
        seq_t K = (seq_t)k;
        seq_t t = (seq_t)2 * (K - 1) * (2 * K - 1) * (2 * K - 1) * x2;
        t += (seq_t)4 * (K - 1) * (2 * K - 3) * x1;
        t -= (seq_t)16 * (K - 2) * (K - 1) * (2 * K - 1) * x0;
        t /= (2 * K - 3);
        x0 = x1;
        x1 = x2;
        x2 = t;
    }
    return x2;
}

/* Inverse of an odd number modulo 2^128 (Newton iteration). */
static unsigned __int128 a316_inv(unsigned __int128 q)
{
    unsigned __int128 x = q;
    int i;
    for (i = 0; i < 7; i++)
        x *= (unsigned __int128)2 - q * x;
    return x;
}

/* Fixed width: work modulo 2^128 and divide by the odd number (2k-3) using its
   modular inverse; this is exact mod 2^w whenever the true value fits. */
static seq_t a316_fixed(idx_t n)
{
    unsigned __int128 x0 = 1, x1 = 0, x2 = 4;
    idx_t k;
    if (n == 0) return (seq_t)x0;
    if (n == 1) return (seq_t)x1;
    if (n == 2) return (seq_t)x2;
    for (k = 3; k <= n; k++) {
        unsigned __int128 K = (unsigned __int128)k;
        unsigned __int128 t = 2 * (K - 1) * (2 * K - 1) * (2 * K - 1) * x2;
        t += 4 * (K - 1) * (2 * K - 3) * x1;
        t -= 16 * (K - 2) * (K - 1) * (2 * K - 1) * x0;
        t *= a316_inv(2 * K - 3);
        x0 = x1;
        x1 = x2;
        x2 = t;
    }
    return (seq_t)x2;
}

seq_t a(idx_t n)
{
    if (seq_is_big())
        return a316_big(n);
    return a316_fixed(n);
}
