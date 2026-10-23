/*
 * A187126  Triangular numbers k*(k+1)/2 mod 1000, sorted and uniqued.
 * https://oeis.org/A187126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is the sorted list of distinct residues modulo 1000 * of
 *            the triangular numbers k*(k+1)/2 (k = 0,1,2,…). The sequence has
 *            * 424 terms, with a(1) = 0 and a(424) = 996. The residues repeat
 *            with * period 2000, so the full set is obtained by scanning k =
 *            0 … 1999
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 424, u64 n <= 424, u128 n <= 424,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Lazy initialization of the sequence on first call. */
    static bool built = false;
    static seq_t seq_list[424];   /* the sorted residues */
    static size_t count = 0;      /* number of residues found */

    if (!built) {
        /* mark which residues have occurred */
        bool seen[1000] = { false };
        for (idx_t k = 0; k < 2000; ++k) {
            seq_t t = (seq_t)k * ((seq_t)k + 1) / 2;
            size_t mod = (size_t)(t % 1000);
            seen[mod] = true;
        }

        /* collect them in ascending order (by increasing residue) */
        for (size_t i = 0; i < 1000; ++i) {
            if (seen[i]) {
                seq_list[count++] = (seq_t)i;
            }
        }
        built = true;
    }

    /* n is 1‑based; convert safely to a size_t offset. */
    if (n == 0) return 0;               /* undefined; keep sane */
    size_t idx = (size_t)(n - 1);
    if (idx >= count) return 0;         /* out of bounds; keep sane */
    return seq_list[idx];
}
