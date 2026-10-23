/*
 * A164666  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=30, a(2)=870, a(3)=25230, a(4)=731670,
   a(5)=21218430, a(6)=615334470, a(7)=17844699195.
   For n ≥ 8:
     a(n) = 28*a(n-1)+28*a(n-2)+28*a(n-3)+28*a(n-4)+28*a(n-5)+28*a(n-6)
            – 406*a(n-7).
   (The case n=7 is handled by the initial value above.) */

#include <stdlib.h>   /* for size_t, if needed */

seq_t a(idx_t n) {
    /* Pre‑computed initial terms up to a(7). */
    static const seq_t init[8] = {
        1ULL,                     /* a(0) */
        30ULL,                    /* a(1) */
        870ULL,                   /* a(2) */
        25230ULL,                 /* a(3) */
        731670ULL,                /* a(4) */
        21218430ULL,              /* a(5) */
        615334470ULL,             /* a(6) */
        17844699195ULL            /* a(7) */
    };

    /* If the requested index is within the initial block, return it directly. */
    if (n < 8) {
        return init[n];
    }

    /* Circular buffer of the last 8 terms (a(n‑7)…a(n‑1)). */
    seq_t buf[8];
    for (int i = 0; i < 8; ++i) {
        buf[i] = init[i];
    }

    /* Compute terms from a(8) up to a(n). */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t sum = 28 * buf[(i - 1) % 8];
        sum += 28 * buf[(i - 2) % 8];
        sum += 28 * buf[(i - 3) % 8];
        sum += 28 * buf[(i - 4) % 8];
        sum += 28 * buf[(i - 5) % 8];
        sum += 28 * buf[(i - 6) % 8];
        sum -= 406 * buf[(i - 7) % 8];
        buf[i % 8] = sum;
    }

    return buf[n % 8];
}
