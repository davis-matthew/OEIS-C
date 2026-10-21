/*
 * A229632  Number of defective 3-colorings of an nX3 0..2 array connected
 *          horizontally, diagonally and antidiagonally with exactly two
 *          mistakes, and colors introduced in row-major 0..2 order.
 * https://oeis.org/A229632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 81*a(n-2) + 185*a(n-3) - 162*a(n-4) +
 *            60*a(n-5) - 8*a(n-6) Initial values: a(1)=1, a(2)=40, a(3)=244,
 *            a(4)=1496, a(5)=8800, a(6)=50084, a(7)=277996
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* Return a(0) as 0 because offset is 1 */
    if (n < 1) return 0;

    /* Initial six terms for recurrence */
    static const seq_t base[7] = {
        (seq_t)1,      /* a(1) */
        (seq_t)40,     /* a(2) */
        (seq_t)244,    /* a(3) */
        (seq_t)1496,   /* a(4) */
        (seq_t)8800,   /* a(5) */
        (seq_t)50084,  /* a(6) */
        (seq_t)277996  /* a(7) */
    };

    if (n <= 7) return base[n-1];

    /* Circular buffer holding last six values:
       buf[0] = a(k-1), buf[1] = a(k-2), ..., buf[5] = a(k-6) */
    seq_t buf[6];
    buf[0] = base[6]; /* a(7)  */
    buf[1] = base[5]; /* a(6)  */
    buf[2] = base[4]; /* a(5)  */
    buf[3] = base[3]; /* a(4)  */
    buf[4] = base[2]; /* a(3)  */
    buf[5] = base[1]; /* a(2)  */

    /* Compute from k=8 to n */
    for (idx_t k = 8; k <= n; ++k) {
        seq_t next;
#ifdef __cplusplus
        if constexpr (std::is_integral<seq_t>::value) {
            /* Use 128‑bit intermediate to avoid overflow on 64‑bit types */
            unsigned __int128 t =
                (unsigned __int128)buf[0] * 15
              - (unsigned __int128)buf[1] * 81
              + (unsigned __int128)buf[2] * 185
              - (unsigned __int128)buf[3] * 162
              + (unsigned __int128)buf[4] * 60
              - (unsigned __int128)buf[5] * 8;
            next = (seq_t)t;
        } else {
            /* seq_t is an arbitrary precision type */
            next = buf[0] * 15
                 - buf[1] * 81
                 + buf[2] * 185
                 - buf[3] * 162
                 + buf[4] * 60
                 - buf[5] * 8;
        }
#else
        /* Pure C implementation using unsigned __int128 */
        unsigned __int128 t =
            (unsigned __int128)buf[0] * 15
          - (unsigned __int128)buf[1] * 81
          + (unsigned __int128)buf[2] * 185
          - (unsigned __int128)buf[3] * 162
          + (unsigned __int128)buf[4] * 60
          - (unsigned __int128)buf[5] * 8;
        next = (seq_t)t;
#endif
        /* Shift buffer to keep last six values */
        buf[5] = buf[4];
        buf[4] = buf[3];
        buf[3] = buf[2];
        buf[2] = buf[1];
        buf[1] = buf[0];
        buf[0] = next;
    }
    /* After loop, buf[0] holds a(n) */
    return buf[0];
}
