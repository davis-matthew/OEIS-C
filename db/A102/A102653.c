/*
 * A102653  a(n) = 4 * floor(9*2^n/5).
 * https://oeis.org/A102653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = 4 * floor(9*2^n/5)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uintptr_t, if needed */

seq_t a(idx_t n)
{
#if defined(__SIZEOF_INT128__) && (__SIZEOF_INT128__ >= 16)
    /* Use 128‑bit arithmetic when seq_t is up to 128 bits and n is small
       enough to avoid UB from left‑shift by >=128.  This handles all
       built‑in integral types (uint8_t … uint128_t). */
    if (sizeof(seq_t) <= 16 && n < 128) {
        unsigned __int128 x = (unsigned __int128)9;
        x <<= n;          /* 9 * 2^n in 128 bits, safe for n < 128   */
        x /= 5;           /* floor division                               */
        x *= 4;           /* final multiplication                       */
        return (seq_t)x;  /* cast back to the requested type */
    }
#endif
    /* Fallback for arbitrary‑precision types or when the 128‑bit path
       cannot be used.  For such types shifting by n is well defined
       and no overflow occurs. */
    seq_t val = (seq_t)9;
    val <<= n;          /* 9 * 2^n (big‑int grows as needed) */
    val /= (seq_t)5;    /* integer floor division */
    val *= (seq_t)4;    /* multiply by 4 */
    return val;
}
