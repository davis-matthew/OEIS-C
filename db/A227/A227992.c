/*
 * A227992  Numbers m such that the continuants of their binary
 *          representations defined in A072347 equal 1.
 * https://oeis.org/A227992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227992: numbers m such that the continuants of their binary
   representations (defined in A072347) equal 1.  The nth term a(n) is
   obtained by scanning successive integers and testing the continuant
   condition. */

#include <stdbool.h>
#include <stdint.h>

static bool good(idx_t m)
{
    if (m == 0)
        return true;                 /* K(empty) = 1 */

    /* Find most significant set bit. */
    int msb = 0;
    for (int b = 63; b >= 0; b--)
    {
        if ((m >> b) & 1ULL) { msb = b; break; }
    }

    idx_t k_pp = 1;   /* K of the empty prefix */
    idx_t k_prev = 0; /* K of the previous prefix (will be set on first bit) */
    bool first = true;

    for (int b = msb; b >= 0; b--)
    {
        idx_t d = (m >> b) & 1ULL;  /* current binary digit (0 or 1) */

        if (first)
        {
            k_prev = d;              /* K = d for the first digit */
            first = false;
        }
        else
        {
            idx_t newk = d * k_prev + k_pp; /* recurrence: K(a1...an)=an*K(prev)+K(prevprev) */
            k_pp = k_prev;
            k_prev = newk;
        }
    }

    return k_prev == 1;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;          /* a(1) = 0 */

    idx_t count = 1;              /* 0 has been counted already */
    idx_t m = 1;

    while (true)
    {
        if (good(m))
        {
            count++;
            if (count == n)
                return (seq_t)m;
        }
        m++;
    }
}
