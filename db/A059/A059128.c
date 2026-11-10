/*
 * A059128  A hierarchical sequence (W3{2,2} - see A059126).
 * https://oeis.org/A059128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(3n+1) = 2*A059126(n), a(3n) = a(3n+2) = a(3n+1) - 1 (checked
 *            against the listed terms; the entry's "1+n" index does not match
 *            them)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Validated: partial, pass, 98 terms through n = 97 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A059126_a(idx_t n)
{
    idx_t t = n / 3 + 1;
    idx_t v = 0;
    while ((t & 1) == 0) {
        v++;
        t >>= 1;
    }
    seq_t base = (seq_t)2 * (seq_t)(v + 1);
    return (n % 3 == 1) ? base : (base - 1);
}

seq_t a(idx_t n)
{
    idx_t m = n / 3;
    seq_t mid = (seq_t)2 * A059126_a(m);
    idx_t r = n % 3;
    if (r == 1) return mid;
    return mid - 1;
}
