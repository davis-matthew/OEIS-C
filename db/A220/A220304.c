/*
 * A220304  Majority value maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements equal to at least half of
 *          their horizontal, vertical and antidiagonal neighbors in a random
 *          0..1 n X 2 array.
 * https://oeis.org/A220304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A220304 – Majority value maps
   Recurrence (valid for n ≥ 11):
     a(n) = 3*a(n-1) + a(n-2) - a(n-3) + 6*a(n-4)
            + 3*a(n-6) + 3*a(n-7) - a(n-8) + 3*a(n-9) - a(n-10)
   Initial values:
     a(1) =   2
     a(2) =   7
     a(3) =  26
     a(4) =  86
     a(5) = 291
     a(6) = 980
     a(7) = 3309
     a(8) =11161
     a(9) =37656
     a(10)=127036 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* offset is 1, value for n<1 is undefined – return 0 */
        return (seq_t)0;
    }

    /* first ten terms – zero based indices 0..9 correspond to a(1)..a(10) */
    static const seq_t init[10] = {
        (seq_t)2,      (seq_t)7,     (seq_t)26,   (seq_t)86,
        (seq_t)291,    (seq_t)980,   (seq_t)3309, (seq_t)11161,
        (seq_t)37656,  (seq_t)127036
    };

    if (n <= 10) {
        return init[n - 1];
    }

    /* buffer holding the last ten values, reversed:
       prev[0] = a(k-1), prev[1] = a(k-2), … , prev[9] = a(k-10) */
    seq_t prev[10];
    for (size_t i = 0; i < 10; ++i) {
        prev[i] = init[9 - i];
    }

    for (idx_t k = 11; k <= n; ++k) {
        seq_t pos = 3 * prev[0]      // 3*a(k-1)
                   + 1 * prev[1]      //   + a(k-2)
                   + 6 * prev[3]      //   + 6*a(k-4)
                   + 3 * prev[5]      //   + 3*a(k-6)
                   + 3 * prev[6]      //   + 3*a(k-7)
                   + 3 * prev[8];     //   + 3*a(k-9)

        seq_t neg = 1 * prev[2]      //   - a(k-3)
                   + 1 * prev[7]      //   - a(k-8)
                   + 1 * prev[9];     //   - a(k-10)

        seq_t new_val = pos - neg;   /* guaranteed non‑negative for all n≥11 */

        /* shift buffer right by one, make room for new_val at prev[0] */
        for (int i = 9; i > 0; --i) {
            prev[i] = prev[i - 1];
        }
        prev[0] = new_val;
    }

    return prev[0];
}
