/*
 * A072795  A014486-indices of the plane binary trees AND plane general trees
 *          whose left subtree is just a stick: \. thus corresponding to the
 *          parenthesizations whose first element (of the top-level list) is
 *          an empty parenthesization: ().
 * https://oeis.org/A072795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + C_k * * Let C_k be the k‑th Catalan number. Find the
 *            unique integer k such * that the cumulative sum of Catalan
 *            numbers satisfies * Σ_{i=0}^{k-1} C_i <= n < Σ_{i=0}^{k} C_i *
 *            (the left sum is the starting index of block k). * Then a(n) is
 *            simply n + C_k. * * The block size equals the C...
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned __int128 cum = 0;   /* cumulative sum of Catalans so far      */
    unsigned __int128 cat = 1;   /* current Catalan number C_k, start C_0   */
    idx_t k = 0;

    while (1) {
        /* Does the current block contain n? */
        if (cum + cat > (unsigned __int128)n) {
            unsigned __int128 res = (unsigned __int128)n + cat;
            return (seq_t)res;
        }

        /* Advance to next block */
        cum += cat;

        /* C_{k+1} = C_k * (4k + 2) / (k + 2)   (integer arithmetic) */
        unsigned __int128 numerator = cat * (unsigned __int128)(4 * k + 2);
        cat = numerator / (k + 2);

        ++k;
    }
}
