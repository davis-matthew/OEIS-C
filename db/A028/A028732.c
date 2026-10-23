/*
 * A028732  Nonsquares mod 19.
 * https://oeis.org/A028732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = 19 * floor((n-1)/9) + r[(n-1) mod 9]
 *  where r = {2,3,8,10,12,13,14,15,18}  (non‑residues mod 19)   */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 is assumed; behaviour for n == 0 is undefined */
    idx_t q = (n - 1) / 9;          /* how many full blocks of 9 */
    idx_t r = (n - 1) % 9;          /* position inside the block */

    /* the 9 nonsquare residues modulo 19, computed once */
    static seq_t nonres[9];
    static bool init = false;
    if (!init) {
        bool is_res[19] = {0};
        for (int x = 0; x < 19; ++x) {
            is_res[(x * x) % 19] = true;
        }
        int j = 0;
        for (int v = 0; v < 19; ++v) {
            if (!is_res[v]) {
                nonres[j++] = (seq_t)v;
            }
        }
        init = true;
    }

    /* compute the n‑th nonsquare modulo 19 */
    return (seq_t)((q * 19) + (idx_t)nonres[(size_t)r]);
}
