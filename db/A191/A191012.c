/*
 * A191012  a(n) = n^5 - n^4 + n^3 - n^2 + n.
 * https://oeis.org/A191012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^5 - n^4 + n^3 - n^2 + n
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 u128;
#else
#error "unsigned __int128 not supported on this compiler"
#endif

seq_t a(idx_t n)
{
    /* n  → u128 to avoid overflow in intermediate products */
    u128 t   = (u128)(seq_t)n;
    u128 n2  = t * t;          /* n^2 */
    u128 n3  = n2 * t;         /* n^3 */
    u128 n4  = n3 * t;         /* n^4 */
    u128 n5  = n4 * t;         /* n^5 */
    u128 res = n5 - n4 + n3 - n2 + t;  /* n^5-n^4+n^3-n^2+n */
    return (seq_t)res;
}
