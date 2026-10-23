/*
 * A031055  Write n in base 8, then complement each digit (d -> 7-d) and
 *          juxtapose.
 * https://oeis.org/A031055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the (position-)th complemented digit of n's base‑8
 *            representation, where the sequence is formed by concatenating
 *            the complemented digits of all base‑8 numbers in order
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    // Find how many full blocks of numbers are required to reach index n.
    // A block of numbers with d octal digits contains 7*8^(d-1) numbers,
    // each contributing d digits.
    __int128 cumul = 0;          // cumulative digits produced so far
    int d = 0;                  // current digit length, starts with 0
    __int128 pow8 = 1;           // 8^(d-1), starts with 8^(-1) = 1

    for (;;)
    {
        int nd = d + 1;
        __int128 count = (__int128)7 * pow8;   // numbers with nd digits
        __int128 add   = count * nd;          // digits contributed by this block
        if (cumul + add >= n)
        {
            d = nd;
            break;
        }
        cumul += add;
        d = nd;
        pow8 *= 8;                // prepare 8^(nd-1) for next iteration
    }

    // Position inside the current block
    __int128 offset      = (__int128)n - cumul - 1;  // 0‑based index
    __int128 number_idx  = offset / d;
    int pos_in_number     = (int)(offset % d);       // 0…d-1, most significant first

    // The actual number whose complemented digits are being read
    __int128 num = pow8 + number_idx;                // num fits in 64 bits

    // Extract base‑8 digits (most significant first)
    uint8_t digits[32];   // enough for 64‑bit integers
    int i = d - 1;
    __int128 tmp = num;
    while (i >= 0)
    {
        digits[i] = (uint8_t)(tmp % 8);
        tmp /= 8;
        i--;
    }

    // Complement digit
    uint8_t comp = 7 - digits[pos_in_number];
    return (seq_t)comp;
}
