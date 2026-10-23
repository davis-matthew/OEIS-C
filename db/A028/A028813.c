/*
 * A028813  Nonsquares mod 100.
 * https://oeis.org/A028813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028813: Nonsquares mod 100 */
/* Computes the nth positive integer whose remainder modulo 100
   is a quadratic nonresidue. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    bool sq[100] = { false };
    idx_t k;

    /* mark all quadratic residues modulo 100 */
    for (k = 0; k < 100; ++k)
        sq[(k * k) % 100] = true;

    /* count how many residues are nonsquares */
    idx_t cnt = 0;
    for (k = 0; k < 100; ++k)
        if (!sq[k]) ++cnt;

    /* position of the desired element within its 100‑block */
    idx_t block   = (n - 1) / cnt;   /* 0‑based block index */
    idx_t offset  = (n - 1) % cnt;   /* 0‑based offset inside the block */

    /* find the offset‑th nonsquare residue */
    idx_t rem = 0;
    for (k = 0; k < 100; ++k)
    {
        if (!sq[k])
        {
            if (offset == 0)
            {
                rem = k;
                break;
            }
            --offset;
        }
    }

    return (seq_t)block * (seq_t)100 + (seq_t)rem;
}
