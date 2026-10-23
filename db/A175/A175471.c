/*
 * A175471  Numbers n such that n^2 contains no digit less than 5.
 * https://oeis.org/A175471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A175471: the n‑th integer whose square
   has no decimal digit less than 5.  The value is found by brute‑force
   search; no memoisation is used so the code works for any seq_t
   (integral or arbitrary‑precision). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range, just return 0 */

    idx_t found = 0;                      /* how many good numbers seen */
    for (idx_t k = 1; ; ++k) {
        seq_t sq = (seq_t)k * (seq_t)k;   /* compute k^2 */

        /* test whether all decimal digits of sq are >= 5 */
        bool ok = true;
        seq_t t = sq;
        while (t > (seq_t)0) {
            unsigned d = (unsigned)(t % (seq_t)10);
            if (d < 5) { ok = false; break; }
            t = t / (seq_t)10;
        }

        if (ok) {
            ++found;
            if (found == n)
                return (seq_t)k;          /* the n‑th term */
        }
    }
}
