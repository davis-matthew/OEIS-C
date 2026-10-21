/*
 * A085537  a(n) = n^4 - n^3.
 * https://oeis.org/A085537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - n^3
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 8351,
 *            u128 n <= 8351, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t pow3(seq_t n)
{
    /* compute n^3 using 128-bit intermediate to avoid overflow on intermediate
       products when seq_t is 64-bit. */
    unsigned __int128 t = (unsigned __int128)n;
    t = t * t * t;
    return (seq_t)t;
}

static inline seq_t pow4(seq_t n)
{
    unsigned __int128 t = (unsigned __int128)n;
    t = t * t * t * t;
    return (seq_t)t;
}

seq_t a(idx_t n)
{
    /* The input index n is unsigned; convert it to seq_t for arithmetic.
       Using 128-bit intermediates where necessary to avoid temporary overflow
       when seq_t is 64‑bit. The result automatically truncates to seq_t,
       which is correct for all implementations supported by the build. */
    seq_t sn = (seq_t)n;
    /* Compute n^4 and n^3 */
    seq_t n_cu = pow3(sn);
    seq_t n_qu = pow4(sn);
    return n_qu - n_cu;
}
