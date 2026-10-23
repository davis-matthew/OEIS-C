/*
 * A024870  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (natural numbers >= 2), t = A023531.
 * https://oeis.org/A024870
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A024870:
   a(n) = Σ_{i=1..⌊n/2⌋} (i+1) * t(n-i+1)
   t(m) = 1 if m = k(k+3)/2 for some k ≥ 1, otherwise 0.
   (t is A023531 – a indicator sequence for numbers of the form k(k+3)/2). */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* values below the offset are irrelevant */

    idx_t k       = n / 2;        /* ⌊n/2⌋  */
    idx_t low     = n - k + 1;    /* the smallest j that contributes */
    seq_t sum = 0;

    for (idx_t i = 1;; ++i) {
        idx_t j = i * (i + 3) / 2; /* m = k(k+3)/2  */
        if (j > n) break;         /* no more terms can contribute */

        if (j < low) continue;    /* this j is too small to appear in the sum */

        /* contribution of the i‑th term: (n-j+2)*1 */
        seq_t term = (seq_t)(n - j + 2);
        sum += term;
    }

    return sum;
}
