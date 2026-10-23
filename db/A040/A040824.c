/*
 * A040824  Continued fraction for sqrt(854).
 * https://oeis.org/A040824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=29; for n>0, a(n)=period[(n‑1)%8] with
 *            period=[4,2,11,4,11,2,4,58]
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */
#include <stdint.h>  /* for uint64_t if needed; seq_t and idx_t are defined elsewhere */

static const seq_t _period[8] = {
    (seq_t)4,
    (seq_t)2,
    (seq_t)11,
    (seq_t)4,
    (seq_t)11,
    (seq_t)2,
    (seq_t)4,
    (seq_t)58
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)29;
    idx_t r = (n - 1) % 8;
    return _period[(size_t)r];
}
