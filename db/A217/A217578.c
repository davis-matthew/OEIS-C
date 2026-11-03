/*
 * A217578  a(n) is the least multiple of n, greater than n, such that all
 *          digits of a(n) are even (resp. odd) if n is even (resp. odd).
 * https://oeis.org/A217578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 197, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 974 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest multiple of n greater than n whose digits
   are all even when n is even, or all odd when n is odd. */
#include <stdbool.h>

static bool digits_parity_ok(seq_t x, bool even_req)
{
    /* Even_req == true  -> all digits must be even
       Even_req == false -> all digits must be odd   */
    while (x)
    {
        unsigned digit = (unsigned)(x % 10);
        if (even_req)
        {
            if (digit & 1)           /* digit is odd */
                return false;
        }
        else
        {
            if (!(digit & 1))        /* digit is even */
                return false;
        }
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;            /* cast index to seq_t */
    bool even_req = (n & 1) == 0;    /* even n -> require even digits */
    seq_t mult = 2;                  /* start with the first multiple > n */
    seq_t result;

    while (1)
    {
        result = n_t * mult;
        if (digits_parity_ok(result, even_req))
            return result;
        mult++;
    }
}
