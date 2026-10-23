/*
 * A138724  Atomic numbers of the periodic table of the elements, read
 *          downwards along 18 columns, up to the 7th period.
 * https://oeis.org/A138724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, tabf
 * Formula:   a(n) = atomic number at position n when reading the periodic *
 *            table downwards along the 18 columns (groups 1‑18), up to period
 *            7. * Missing entries are omitted. The sequence has 90 terms
 *            (atomic numbers * 1–118, excluding the lanthanide block 58–71,
 *            actinide block 90–103, * and numbers >118). Th...
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static const seq_t periodic_table[7][18] = {
    /* Period 1 */
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    /* Period 2 */
    { 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 7, 8, 9,10},
    /* Period 3 */
    {11,12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,13,14,15,16,17,18},
    /* Period 4 */
    {19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36},
    /* Period 5 */
    {37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54},
    /* Period 6 (lanthanides omitted) */
    {55,56,57,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86},
    /* Period 7 (actinides omitted) */
    {87,88,89,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118}
};

seq_t a(idx_t n)
{
    /* The sequence has 90 terms; build once and cache. */
    static seq_t seq[90];
    static bool built = false;

    if (!built) {
        idx_t pos = 0;
        /* Read column by column (groups 1..18), then down periods 1..7. */
        for (idx_t g = 0; g < 18; ++g)         /* column (group) */
            for (idx_t p = 0; p < 7; ++p) {    /* row (period) */
                seq_t v = periodic_table[p][g];
                if (v) seq[pos++] = v;        /* skip missing entries */
            }
        built = true;
        /* pos should now be 90; we ignore potential mismatch as the
           construction follows the OEIS definition. */
    }

    if (n < 1 || n > 90) return 0;   /* undefined outside the sequence */
    return seq[n - 1];
}
