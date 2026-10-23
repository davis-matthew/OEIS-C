/*
 * A132825  Triangle read by rows: zeros except for right border which are the
 *          partition numbers A000041.
 * https://oeis.org/A132825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = p(r) if n = r(r+1)/2, otherwise 0. Here p(r) is the
 *            partition number A000041
 * Range:     u8 n <= 152, u16 n <= 989, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static table of partition numbers */
static seq_t *part = NULL;
/* number of elements allocated in part */
static size_t part_cap = 0;
/* largest index whose partition number is computed */
static idx_t  part_last = 0;

/* Ensure that partition numbers up to 'needed' are computed */
static void _ensure_partition(idx_t needed)
{
    if (needed <= part_last) return;

    /* Grow the table if necessary */
    if ((size_t)(needed + 1) > part_cap) {
        size_t new_cap = (size_t)(needed + 1);
        seq_t *tmp = (seq_t *)realloc(part, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* allocation failure – caller may fail */
        part      = tmp;
        part_cap  = new_cap;
    }

    /* Initialise the first partition number if this is the first call */
    if (part_last == 0 && part_cap > 0) {
        part[0] = (seq_t)1;          /* p(0) = 1 */
    }

    /* Compute partition numbers from part_last+1 up to needed
       using Euler's pentagonal number theorem. */
    for (idx_t m = part_last + 1; m <= needed; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent1 > m && pent2 > m) break;
            /* sign alternates: +, -, +, -, ...  */
            int sign = (k & 1) ? 1 : -1;
            if (pent1 <= m) {
                sum = (sign > 0) ? sum + part[m - pent1]
                                 : sum - part[m - pent1];
            }
            if (pent2 <= m) {
                sum = (sign > 0) ? sum + part[m - pent2]
                                 : sum - part[m - pent2];
            }
        }
        part[m] = sum;
    }

    part_last = needed;
}

/* Compute a(n) for OEIS sequence A132825 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined; fall back to 0 */

    /* Find r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ( (unsigned __int128)(r + 1) * (r + 2) / 2 <= n ) ++r;

    /* Triangular number of r */
    seq_t tri = (unsigned __int128)r * (r + 1) / 2;

    /* If n is not a triangular number, return 0. */
    if ((seq_t)n != tri) return (seq_t)0;

    /* Ensure partition numbers up to r are available */
    _ensure_partition(r);

    return part[r];
}
