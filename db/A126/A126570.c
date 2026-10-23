/*
 * A126570  Triangle read by rows: row n gives coefficients (ignoring the
 *          alternating signs) of the characteristic polynomial of the n X n
 *          matrix with 2's in the main diagonal, 1's in the super- and
 *          subdiagonals, and 1 in the upper-right corner, with other elements
 *          zeros.
 * https://oeis.org/A126570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A126570: Triangle read by rows.
   Row n (n ≥ 0) contains the coefficients (ignoring alternating signs)
   of the characteristic polynomial of the nxn matrix with 2 on the
   diagonal, 1 on the sub‑ and super‑diagonals, and 1 in the upper‑right
   corner.  For n > 2 the general term of the triangle is

       T(n,k) = binomial(2n+1‑k, k) – [k = n]·(−1)ⁿ     (0 ≤ k ≤ n),

   where [k = n] is 1 if k = n and 0 otherwise.  The sequence a(n) is
   obtained by reading the triangle row by row (row 0 first, then row 1,
   etc.).

   The function below calculates a(n) for any non‑negative n.  It uses
   the provided utilities:
     - floor_isqrt_u64()     for integer square root,
     - binomial()            for binomial coefficients with arbitrary
       precision output (seq_t type).
*/

/* Include the required utilities */
#include "../utils/floor_isqrt_u64.h"  /* floor_isqrt_u64() */
#include "../utils/binomial.h"        /* binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Determine row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
       r = floor((√(8n+1) – 1)/2).                                         */
    idx_t s = floor_isqrt_u64(8 * n + 1);
    idx_t r = (s - 1) / 2;      /* row index (0‑based) */

    /* Offset of the first element of row r in the linearized array.   */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;       /* column within the row (0‑based) */

    /* Handle the first three rows explicitly (formula fails for n ≤ 2). */
    if (r == 0) {
        return (seq_t)1;
    }
    if (r == 1) {
        return (k == 0) ? (seq_t)1 : (seq_t)2;
    }
    if (r == 2) {
        if (k == 0) return (seq_t)1;
        if (k == 1) return (seq_t)4;
        return (seq_t)3;     /* k == 2 */
    }

    /* For r ≥ 3 use the general formula.                                 */
    seq_t val = binomial(2 * r + 1 - k, k);

    /* Special adjustment for the last element of each row.                */
    if (k == r) {
        if ((r & 1ULL) == 0ULL) {
            val = val - (seq_t)1;   /* n even: subtract 1 */
        } else {
            val = val + (seq_t)1;   /* n odd:  add 1  */
        }
    }

    return val;
}
