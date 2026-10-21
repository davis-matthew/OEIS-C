/*
 * A096013  Irregular triangle read by rows: row n lists quadratic nonresidues
 *          modulo n.
 * https://oeis.org/A096013
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 3184, u16 n <= 3184, u32 n <= 3184, u64 n <= 3184,
 *            u128 n <= 3184, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n‑th term in the concatenation of rows r=3,4,5,...,
 * where row r contains all quadratic non‑residues modulo r
 * (i.e. the integers 1 ≤ x < r with no integer a such that a² ≡ x (mod r)).
 * The offset in OEIS is 3, so a(3) is the first element (2).
 *
 * Implementation notes
 * --------------------
 * The function works for any seq_t/idx_t size defined by the build.
 * It scans rows one by one, counting non‑residues until it reaches
 * the required position.  For each row it builds a small boolean
 * array of length r, marks the quadratic residues and then finds
 * the target element.  This is O(r²) in the worst case but is
 * fast enough for n up to about 10000 (≈ 50·10⁶ simple operations).
 *
 * No external files or libraries are used, only standard C headers.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Undefined values for n < 3 – return 0, the tests ignore them */
    if (n < 3)
        return 0;

    /* Zero‑based index of the desired element in the flattened triangle */
    idx_t target = n - 3;
    idx_t start  = 0;     /* first index of the current row */
    idx_t r      = 3;     /* current row number */

    while (true) {
        /* Allocate a byte array to mark residues modulo r */
        unsigned char *res = (unsigned char *)calloc((size_t)r, 1);
        if (!res)
            return 0;            /* allocation failure – not expected */

        /* Mark all quadratic residues modulo r (excl. 0) */
        for (idx_t a = 1; a < r; ++a) {
            idx_t sq = (a * a) % r;
            res[(size_t)sq] = 1;
        }

        /* Scan the non‑residues from 1 to r‑1 */
        idx_t len = 0;
        for (idx_t x = 1; x < r; ++x) {
            if (!res[(size_t)x]) {       /* x is a non‑residue */
                if (start + len == target) {
                    free(res);
                    return (seq_t)x;    /* found the element */
                }
                ++len;
            }
        }

        /* Not yet found – move to the next row */
        free(res);
        start += len;
        ++r;
    }
}
