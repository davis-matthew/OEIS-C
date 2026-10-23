/*
 * A025702  Index of 4^n within the sequence of the numbers of the form
 *          4^i*5^j.
 * https://oeis.org/A025702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{k=0..n} ceil(k * log_5(4)) (c = log_5(4))
 * Range:     u8 n <= 23, u16 n <= 389, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <math.h>

static const unsigned long long SCALE = ((unsigned long long)1 << 60);
static const unsigned long long P = (unsigned long long)( (logl(4.0L) / logl(5.0L)) * SCALE + 0.5L );

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t value = 1;
    for (idx_t k = 1; k <= n; ++k) {
        unsigned __int128 prod = (unsigned __int128)k * P;
        seq_t floor_kc = (seq_t)(prod / SCALE);
        value += floor_kc + (seq_t)1;
    }
    return value;
}
