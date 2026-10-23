/*
 * A182042  Triangle T(n,k), read by rows, given by (0, 2, -1/2, 1/2, 0, 0, 0,
 *          0, 0, 0, 0, ...) DELTA (3, 0, -3/2, 3/2, 0, 0, 0, 0, 0, 0, 0, ...)
 *          where DELTA is the operator defined in A084938.
 * https://oeis.org/A182042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 181, u64 n <= 617,
 *            u128 n <= 2256, big any n
 * Helpers:   binom, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A182042: a(n) = T(r,k) where r = ⌊(√(8n+1)-1)/2⌋,
   k = n - r(r+1)/2, and T(r,0)=0^r (0^0=1),  T(r,k)=C(r,k)*3^k for k>0 */

#include "../utils/binom.h"  /* binom(idx_t a, idx_t b) -> seq_t */
#include "../utils/pow3.h"   /* pow3(idx_t e) -> seq_t */

seq_t a(idx_t n)
{
    /* Determine the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    /* r grows only up to ~√(2n) which is < 200 for n≤10000, so a tight loop is fine. */
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Linear offset of the first element of row r. */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;                 /* column index within row r */

    if (k == 0) {                         /* T(r,0) = 0^r, with 0^0 = 1 */
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    }

    seq_t bin = binom(r, k);              /* C(r,k) */
    seq_t pw  = pow3(k);                  /* 3^k */

    return bin * pw;                      /* T(r,k) = C(r,k)*3^k */
}
