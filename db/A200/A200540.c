/*
 * A200540  Product of Pell and Motzkin numbers: a(n) =
 *          A000129(n+1)*A001006(n).
 * https://oeis.org/A200540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000129(n+1) * A001006(n) * * A000129 (Pell numbers):
 *            P(0)=0, P(1)=1, P(n)=2*P(n-1)+P(n-2) (n≥2) * A001006 (Motzkin
 *            numbers): M(0)=1, M(1)=1, * (n+2)·M(n) = (2n+1)·M(n-1) +
 *            (3n-3)·M(n-2) (n≥2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   get_motzkin, get_pell
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_motzkin.h"
#include "../utils/get_pell.h"

/* Sequence function required by the build. */
seq_t a(idx_t n) {
    /* a(n) = Pell(n+1) * Motzkin(n) */
    seq_t pell = get_pell(n + (idx_t)1);
    seq_t motz = get_motzkin(n);
    return pell * motz;
}
