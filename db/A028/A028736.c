/*
 * A028736  Nonsquares mod 23.
 * https://oeis.org/A028736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A028736 – Nonsquares mod 23
    a(n) = arr[(n-1)%11] + 23*((n-1)/11),
    where arr = {5,7,10,11,14,15,17,19,20,21,22} (the 11 non‑quadratic residues mod 23). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* lazily build the list of the 11 nonsquare residues modulo 23 */
    static bool initialized = false;
    static seq_t residues[11];

    if (!initialized) {
        bool is_square[23] = {false};
        idx_t x;

        for (x = 1; x < 23; ++x) {
            idx_t sq = (x * x) % 23;
            is_square[sq] = true;
        }

        idx_t k = 0;
        for (x = 1; x < 23; ++x) {
            if (!is_square[x]) {
                residues[k++] = (seq_t)x;
            }
        }
        /* k should be 11 for a prime 23 */
        initialized = true;
    }

    if (n == 0) return 0;            /* Behaviour for indexes below the offset */

    idx_t t   = n - 1;                /* shift to 0‑based index   */
    idx_t rem = t % 11;               /* position within a cycle   */
    idx_t quo = t / 11;               /* number of completed cycles */

    /* Compute in seq_t to avoid overflow of intermediate calculations */
    seq_t shift = (seq_t)quo * (seq_t)23;
    return residues[rem] + shift;
}
