/*
 * A207683  Number of 3 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 6*a(n-3) + 4*a(n-4) – 6*a(n-6), for n > 8 with
 *            initial values a(1)=6, a(2)=36, a(3)=72, a(4)=164, a(5)=336,
 *            a(6)=702, a(7)=1488, a(8)=3164. Off‑sight value a(0) is unused
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 26, u64 n <= 56, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Precomputed small values (index 0 unused). */
    static const seq_t BASE[9] = {
        (seq_t)0,    /* a(0) – unused */
        (seq_t)6,    /* a(1) */
        (seq_t)36,   /* a(2) */
        (seq_t)72,   /* a(3) */
        (seq_t)164,  /* a(4) */
        (seq_t)336,  /* a(5) */
        (seq_t)702,  /* a(6) */
        (seq_t)1488, /* a(7) */
        (seq_t)3164  /* a(8) */
    };

    if (n == 0)
        return (seq_t)0;           /* Undefined below offset – return 0 */

    if (n <= 8)
        return BASE[n];            /* Initial values cover up to 8 */

    /* Use a compact circular buffer of size 9 to hold the last
       9 computed terms (indices 0..8).  Indices 1–8 are the initial
       values; index 0 is used only when (i % 9 == 0).  */
    seq_t vals[9];
    /* Initialise indices 1..8. */
    for (idx_t i = 1; i <= 8; ++i)
        vals[i] = BASE[i];

    /* Compute terms from 9 up to n. */
    for (idx_t i = 9; i <= n; ++i) {
        seq_t t = vals[(i - 2) % 9];
        t += (seq_t)6 * vals[(i - 3) % 9];
        t += (seq_t)4 * vals[(i - 4) % 9];
        t -= (seq_t)6 * vals[(i - 6) % 9];
        vals[i % 9] = t;
    }

    return vals[n % 9];
}
