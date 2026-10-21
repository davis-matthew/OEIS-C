/*
 * A056328  Number of reversible string structures with n beads using exactly
 *          four different colors.
 * https://oeis.org/A056328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the linear recurrence * a(n) = 8 a(n-1) - 12 a(n-2)
 *            - 44 a(n-3) + 121 a(n-4) + 12 a(n-5) - 228 a(n-6) + 144 a(n-7) *
 *            for n ≥ 8, with initial values * a(1)=0, a(2)=0, a(3)=0, a(4)=1,
 *            a(5)=6, a(6)=37, a(7)=183. * The recurrence is derived from the
 *            rational generating function * A(x)=x^...
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values (1‑based). */
    static seq_t *buf = NULL;          /* buffer of a(1)...a(k) */
    static size_t buf_size = 0;        /* allocated size of buffer */
    static size_t buf_max   = 0;       /* number of terms currently stored (max index) */

    /* First time: initialise buffer with the first 7 terms. */
    if (buf == NULL) {
        /* Allocate space for indices 0..7 (index 0 unused). */
        buf_size = 8;
        buf = (seq_t *)malloc(buf_size * sizeof(seq_t));
        if (!buf) return 0;           /* allocation failed: return 0 as safe default */

        buf[0] = 0;                /* unused */
        buf[1] = (seq_t)0;
        buf[2] = (seq_t)0;
        buf[3] = (seq_t)0;
        buf[4] = (seq_t)1;
        buf[5] = (seq_t)6;
        buf[6] = (seq_t)37;
        buf[7] = (seq_t)183;
        buf_max = 7;
    }

    /* If requested index within cache, return it directly. */
    if (n <= buf_max) {
        return buf[n];
    }

    /* Extend the cache up to n using the recurrence. */
    size_t target = (size_t)n;
    for (size_t i = buf_max + 1; i <= target; ++i) {
        /* Ensure the buffer is large enough. */
        if (i + 1 > buf_size) {
            size_t new_size = (i + 1) * 2;
            seq_t *new_buf = (seq_t *)realloc(buf, new_size * sizeof(seq_t));
            if (!new_buf) return 0;   /* reallocation failed */
            buf     = new_buf;
            buf_size = new_size;
        }

        /* Fetch the seven previous terms. */
        seq_t a1 = buf[i-1];
        seq_t a2 = buf[i-2];
        seq_t a3 = buf[i-3];
        seq_t a4 = buf[i-4];
        seq_t a5 = buf[i-5];
        seq_t a6 = buf[i-6];
        seq_t a7 = buf[i-7];

        /* Compute the recurrence using only unsigned operations. */
        seq_t t1 = (seq_t)8   * a1;
        seq_t t2 = (seq_t)12  * a2;
        seq_t t3 = (seq_t)44  * a3;
        seq_t t4 = (seq_t)121 * a4;
        seq_t t5 = (seq_t)12  * a5;
        seq_t t6 = (seq_t)228 * a6;
        seq_t t7 = (seq_t)144 * a7;

        /* Positive contributions. */
        seq_t val = t1 + t4 + t5 + t7;
        /* Subtract the negative contributions. */
        val = val - t2;
        val = val - t3;
        val = val - t6;

        buf[i] = val;
    }

    buf_max = target;
    return buf[n];
}
