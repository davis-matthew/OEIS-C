/*
 * A073475  Rearrangement of factorial numbers such that a(k) + a(k+1) - 1 is
 *          a prime for all k.
 * https://oeis.org/A073475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A finite permutation of factorial numbers:
 * 1, 2, 6, 24, 720, 120.
 * For n ≤ 6 the function returns the n‑th term of the permutation;
 * for n > 6 it returns 0.
 * The factorials are computed once and reused on subsequent calls. */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* factorial values 1! .. 6! (index 0 unused) */
    static seq_t fact[7];
    static bool init = false;

    if (!init) {
        fact[0] = (seq_t)1;          /* dummy */
        seq_t prod = (seq_t)1;
        for (idx_t i = 1; i <= 6; ++i) {
            prod = prod * (seq_t)i;  /* compute i! */
            fact[i] = prod;
        }
        init = true;
    }

    /* permutation order: 1, 2, 3, 4, 6, 5 */
    static const idx_t order[6] = {1, 2, 3, 4, 6, 5};

    if (n >= 1 && n <= 6U) {
        idx_t k = order[(size_t)(n - 1)];
        return fact[k];
    }
    return (seq_t)0;
}
