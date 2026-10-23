/*
 * A028733  Nonsquares mod 20.
 * https://oeis.org/A028733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A028733: a(n) = the nth positive integer that is *not* a quadratic
   residue modulo 20.  The set of residues 0..19 that are quadratic
   residues modulo 20 is {0,1,4,5,9,16}.  The non‑residues are
   2,3,6,7,8,10,11,12,13,14,15,17,18,19.  For n ≥ 1 we compute
   a(n) = 20·⌊(n‑1)/14⌋ plus the ((n‑1) mod 14)‑th non‑residue.   */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 0‑based index within the sequence */
    idx_t k = n - 1;
    /* how many complete blocks of 20 numbers (each block contributes
       14 non‑residues) */
    idx_t cycle = k / 14;
    /* position inside the current block (0‑based) */
    idx_t offset = k % 14;

    /* lazily build table of quadratic residues modulo 20 */
    static bool qr[20];
    static int qr_inited = 0;
    if (!qr_inited) {
        for (idx_t i = 0; i < 20; ++i) qr[i] = false;
        for (idx_t i = 0; i < 20; ++i) qt: /* mark residues */
            qr[(i * i) % 20] = true;
        qr_inited = 1;
    }

    /* find the (offset)-th non‑residue */
    idx_t r = 0;
    idx_t seen = 0;
    for (r = 0; r < 20; ++r) {
        if (!qr[r]) {
            if (seen == offset) break;
            ++seen;
        }
    }

    /* combine to yield the final integer value */
    return (seq_t)((seq_t)20 * (seq_t)cycle + (seq_t)r);
}
