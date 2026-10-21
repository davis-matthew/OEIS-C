/*
 * A073999  Number of strings of length n over GF(4) with trace 1 and subtrace
 *          x where x = RootOf(z^2+z+1).
 * https://oeis.org/A073999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n ≤ 2; if n is odd: a(n) = 4^(n‑2) – 2^(n‑3); if n
 *            ≡ 0 (mod 4): a(n) = 4^(n‑2); if n ≡ 2 (mod 4): a(n) = 4^(n‑2) –
 *            2^(n‑2)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n <= 2) {
        return 0;
    }

    /* compute 4^(n-2) = 2^(2n-4) once */
    seq_t pow4 = pow_int((seq_t)2, 2 * n - 4);

    if (n & 1) {                    /* n is odd */
        seq_t pow2 = pow_int((seq_t)2, n - 3);
        return pow4 - pow2;
    } else {                        /* n is even */
        if ((n >> 1) & 1) {         /* n ≡ 2 (mod 4) */
            seq_t pow2 = pow_int((seq_t)2, n - 2);
            return pow4 - pow2;
        } else {                    /* n ≡ 0 (mod 4) */
            return pow4;
        }
    }
}
