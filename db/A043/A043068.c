/*
 * A043068  a(n)=(s(n)+4)/8, where s(n)=n-th base 8 palindrome that starts
 *          with 4.
 * https://oeis.org/A043068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+4)/8 where s(n) is the n-th base‑8 palindrome that
 *            starts with the digit 4
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* The types seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    if (n == 0)       /* the sequence is defined for n ≥ 1 only */
        return (seq_t)0;

    /* Find the half‑length (ceil(L/2)) that contains the n‑th palindrome. */
    idx_t rem = n;          /* remaining index to locate */
    idx_t h   = 1;          /* current half‑length */
    idx_t pow = 1;          /* 8^(h-1): number of left‑half choices */

    while (rem > 2 * pow)
    {
        rem -= 2 * pow;    /* skip the whole current group */
        pow *= 8;          /* next group has 8× more combinations */
        ++h;
    }

    /* Position within the current group (0‑based). */
    idx_t group_index = rem - 1;
    bool  odd         = group_index < pow;          /* odd or even length */
    idx_t leftIdx     = odd ? group_index : group_index - pow; /* left‑half index */

    /* Build the h digits of the left‑half. The most significant digit is fixed to 4. */
    unsigned char digits[64];   /* enough for all possible h (≤6 for n≈10⁴) */
    digits[0] = 4;
    idx_t tmp = leftIdx;
    for (idx_t i = h - 1; i >= 1; --i)
    {
        digits[i] = (unsigned char)(tmp % 8);
        tmp      /= 8;
    }

    /* Construct the palindrome in decimal. */
    seq_t val = 0;
    for (idx_t i = 0; i < h; ++i)
        val = val * ((seq_t)8) + (seq_t)digits[i];

    int mirror_start = odd ? (int)h - 2 : (int)h - 1;
    for (int i = mirror_start; i >= 0; --i)
        val = val * ((seq_t)8) + (seq_t)digits[i];

    /* Return a(n) = (s(n)+4)/8. */
    return (val + ((seq_t)4)) / ((seq_t)8);
}
