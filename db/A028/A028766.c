/*
 * A028766  Nonsquares mod 53.
 * https://oeis.org/A028766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Nonsquares mod 53
   a(n) is the nth positive integer whose remainder modulo 53 is a
   quadratic non‑residue (i.e. Legendre symbol (a|53) = –1). */
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Ensure the table of non‑residues modulo 53 is built once. */
    static int init = 0;
    static seq_t nonres[26];

    if (!init) {
        init = 1;
        idx_t cnt = 0;
        for (idx_t i = 1; i < 53; ++i) {
            seq_t r = mod_pow((seq_t)i, 26, (seq_t)53); /* i^((53-1)/2) mod 53 */
            if (r == (seq_t)1)           /* quadratic residue, skip */
                continue;
            /* For prime 53, the only remaining non‑zero symbol is 52 (= –1) */
            nonres[cnt++] = (seq_t)i;
        }
        /* cnt should be 26, but we ignore any check on the caller side. */
    }

    idx_t pos   = (n - 1) % 26;   /* position within the remainder cycle */
    idx_t block = (n - 1) / 26;   /* which 53‑block we are in */

    return (seq_t)block * (seq_t)53 + nonres[pos];
}
