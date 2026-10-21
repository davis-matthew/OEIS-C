/*
 * A062850  When expressed in base 2 and then interpreted in base 9, is a
 *          multiple of the original number.
 * https://oeis.org/A062850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   mul9_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) = numbers x such that interpreting the binary
   representation of x as a base‑9 number yields a multiple of x. */

#include <stdint.h>
#include "../utils/mul9_mod.h"

/* Maximum number of terms we will store.
   300 000 comfortably covers all n up to about 10 000. */
#define MAX_TERMS 300000

/* 1‑based array of terms */
static seq_t seq_arr[MAX_TERMS + 1];
static idx_t seq_len = 0;          /* number of stored terms */
static uint64_t next_x = 0;        /* smallest number not yet tested */

/* Find the next qualifying number and store it in seq_arr. */
static void find_next(void)
{
    for (uint64_t x = next_x; ; ++x) {
        /* Special case: 0 is regarded as a member of the sequence. */
        if (x == 0) {
            seq_arr[++seq_len] = (seq_t)0;
            next_x = 1;
            return;
        }

        seq_t y_mod = 0;
        seq_t pow9_mod = 1 % (seq_t)x;    /* 9^0 mod x = 1 mod x */

        uint64_t tmp = x;
        while (tmp) {
            if (tmp & 1) {
                y_mod += pow9_mod;
                if (y_mod >= (seq_t)x)
                    y_mod -= (seq_t)x;
            }
            /* Update pow9_mod = (pow9_mod * 9) % x */
            pow9_mod = mul9_mod(pow9_mod, (seq_t)x);
            tmp >>= 1;
        }

        if (y_mod == 0) {
            seq_arr[++seq_len] = (seq_t)x;
            next_x = x + 1;
            return;
        }
    }
}

/* Return the n‑th term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; offset starts at 1 */

    while (seq_len < n) {
        /* Generate terms until we have at least n of them. */
        find_next();
    }
    return seq_arr[n];
}
