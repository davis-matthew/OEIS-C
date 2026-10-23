/*
 * A143230  Triangle read by rows, A130207 * A000012 * A130207.
 * https://oeis.org/A143230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = φ(r) * φ(k) where r is the unique row index such that
 *            r(r+1)/2 ≥ n and k = n − (r−1)r/2 (1‑based column within the
 *            row). Here φ(m) is Euler’s totient function
 * Range:     u8 n <= 152, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Find the row r (1‑based) containing index n. */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (cumulative < n)
    {
        r++;
        cumulative += r;            /* cumulative = r(r+1)/2 after the loop        */
    }

    /* Column index k (1‑based). */
    idx_t k = n - (cumulative - r); /* cumulative - r = (r-1)r/2  */

    /* Compute φ(r) * φ(k). */
    seq_t phir = phi(r);
    seq_t phik = phi(k);
    return phir * phik;
}
