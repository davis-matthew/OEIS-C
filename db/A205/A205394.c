/*
 * A205394  Least k such that n divides s(k)-s(j) for some j<k, where
 *          s(j)=ceiling(j^2/2).
 * https://oeis.org/A205394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Sequence A205394:  a(n) = Least k such that n divides s(k)-s(j) for some j<k,
   where s(j) = ceil(j^2/2).  Computed by detecting the first repetition of
   the residues s(k) mod n, i.e. the smallest k with s(k) ≡ s(j) (mod n)
   for some j<k.
*/
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* n is always ≥1 in the OEIS offset */

    idx_t m = 2 * n;                     /* modulus used to avoid overflow   */
    size_t sz = (size_t)n;               /* array size for seen residues     */
    bool *seen = (bool *)calloc(sz, sizeof(bool));
    if (!seen) return 0;                 /* allocation failure (unlikely)   */

    for (idx_t k = 1; k <= n + 1; ++k) {
        idx_t km = k % m;
        idx_t t  = (km * km) % m;        /* k^2 mod 2n, stays small          */
        idx_t s  = (t + 1) / 2;          /* ceil(k^2/2) mod 2n, integer div  */
        idx_t r  = s % n;                /* residue modulo n                 */
        if (seen[r]) {
            free(seen);
            return (seq_t)k;            /* first duplicate found            */
        }
        seen[r] = true;
    }

    free(seen);
    return (seq_t)(n + 1);                /* should never be reached           */
}
