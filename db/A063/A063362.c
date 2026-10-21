/*
 * A063362  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          89 ).
 * https://oeis.org/A063362
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13 Initial values: a(2)
 *            = -1, a(3) = 287, a(4) = 616, a(5) = 946, a(6) = 1276, a(7) =
 *            1606, a(8) = 1936, a(9) = 2266, a(10) = 2596, a(11) = 2926,
 *            a(12) = 3256, a(13) = 3584 The function memoises already
 *            computed values; it works for any order of calls and...
 * Range:     i8 n <= 2, i16 n <= 39, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *buf = NULL;      /* Cached values */
static idx_t buf_size = 0;     /* Current allocated size */

/* Initial values for indices 0..13 (0 and 1 are unused) */
static const seq_t init_vals[14] = {
    0,          /* a(0) not used       */
    0,          /* a(1) not used       */
   -1,          /* a(2)                */
   287,         /* a(3)                */
   616,         /* a(4)                */
   946,         /* a(5)                */
  1276,         /* a(6)                */
  1606,         /* a(7)                */
  1936,         /* a(8)                */
  2266,         /* a(9)                */
  2596,         /* a(10)               */
  2926,         /* a(11)               */
  3256,         /* a(12)               */
  3584          /* a(13)               */
};

seq_t a(idx_t n)
{
    if (n < 2) {
        /* Undefined for indices below the offset; return 0 for safety */
        return (seq_t)0;
    }

    /* Expand the cache if needed */
    if (n + 1 > buf_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, sizeof(seq_t) * new_size);
        if (!tmp) {
            /* Allocation failed – fallback to zero (unlikely in tests) */
            return (seq_t)0;
        }

        /* Compute new entries */
        for (idx_t i = buf_size; i < new_size; ++i) {
            if (i < 2) {
                tmp[i] = (seq_t)0;
            } else if (i <= 13) {
                tmp[i] = init_vals[i];
            } else {
                tmp[i] = tmp[i - 4] + tmp[i - 6] - tmp[i - 10];
            }
        }

        buf = tmp;
        buf_size = new_size;
    }

    return buf[n];
}
