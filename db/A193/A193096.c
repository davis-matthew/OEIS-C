/*
 * A193096  Numbers that cannot be written as concatenation of two nonzero
 *          squares in decimal representation.
 * https://oeis.org/A193096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A193096 - Numbers that cannot be written as concatenation
   of two nonzero squares in decimal representation.
   Formula: a(n) = the nth non‑concatenating number (a(1)=0). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/pow10.h"

/* Test whether x is a non‑zero perfect square */
static bool is_square(seq_t x)
{
    if (x == 0)
        return false;
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

/* Test whether x can be represented as the concatenation of
   two non‑zero squares.  The concatenation is performed on the
   decimal representation of the squares.  A split that would
   produce a leading zero in the second part is ignored. */
static bool is_concat_square(seq_t x)
{
    /* obtain decimal digits into an array (most significant first) */
    uint8_t d[40];           /* enough for < 128‑bit */
    int len = 0;
    seq_t t = x;
    if (t == 0) {
        d[0] = 0;
        len = 1;
    } else {
        while (t > 0) {
            d[len++] = (uint8_t)(t % 10);
            t /= 10;
        }
        /* reverse to get most significant digit first */
        for (int i = 0; i < len / 2; ++i) {
            uint8_t tmp = d[i];
            d[i] = d[len - 1 - i];
            d[len - 1 - i] = tmp;
        }
    }

    if (len < 2)
        return false;   /* cannot split into two non‑empty parts */

    for (int pos = 1; pos < len; ++pos) {
        /* skip splits where the first digit of the right part is 0 */
        if (d[pos] == 0) continue;

        seq_t pow = pow10(len - pos);
        seq_t left  = x / pow;
        seq_t right = x % pow;
        if (left == 0 || right == 0) continue;
        if (is_square(left) && is_square(right))
            return true;
    }
    return false;
}

/* Return the nth term of A193096 (1‑based). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 0;
    while (true) {
        if (!is_concat_square(x)) {
            ++count;
            if (count == n)
                return x;
        }
        ++x;
    }
}
