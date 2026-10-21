/*
 * A221763  Numbers m such that 11*m^2 - 7 is a square.
 * https://oeis.org/A221763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=4, a(3)=16, a(4)=79 and for n≥5 a(n) = 20*a(n‑2) –
 *            a(n‑4) (recurrence derived from the OEIS entry)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Not defined for n = 0; return 0 just to avoid UB */
    if (n == 0) return (seq_t)0;

    /* Handle first four terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)16;
    if (n == 4) return (seq_t)79;

    /* Buffer holding the last four computed values.
       Indices in this buffer correspond to a(k) modulo 4. */
    seq_t last[4] = { (seq_t)1, (seq_t)4, (seq_t)16, (seq_t)79 };

    /* Compute iteratively up to the requested index.  i is 0‑based,
       so i = 4 corresponds to the 5th term (n = 5). */
    for (idx_t i = 4; i < n; ++i) {
        idx_t idx_m2 = (i - 2) % 4;   // index of a(n-2)
        idx_t idx_m4 = (i - 4) % 4;   // index of a(n-4)

        /* a(n) = 20*a(n-2) - a(n-4) */
        seq_t val = (seq_t)20 * last[idx_m2] - last[idx_m4];
        last[i % 4] = val;
    }

    return last[(n - 1) % 4];
}
