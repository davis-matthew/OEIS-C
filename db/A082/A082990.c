/*
 * A082990  Rent due for landing on undeveloped Monopoly properties starting
 *          at Mediterranean Avenue and finishing at Boardwalk, not including
 *          railroads or utilities.
 * https://oeis.org/A082990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) is the level‑0 rent of the n‑th undeveloped Monopoly
 *            property (excluding railroads and utilities). It is obtained by
 *            mapping n to its color group and position within that group
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Only 22 undeveloped properties exist. For n >= 22 return 0. */
    if (n >= 22)
        return (seq_t)0;

    /* Sizes of the color groups in order:
       2 (Brown), 3 (Light Blue), 3 (Pink), 3 (Orange), 3 (Red),
       3 (Yellow), 3 (Green), 2 (Dark Blue). */
    const idx_t group_sizes[8] = {2,3,3,3,3,3,3,2};

    /* Find the group that contains the n‑th property. */
    idx_t remaining = n;
    idx_t g = 0;
    while (remaining >= group_sizes[g]) {
        remaining -= group_sizes[g];
        ++g;
    }
    /* remaining is the position within the group. */
    idx_t pos = remaining;

    if (g == 0) {
        /* Brown group: 2, 4 */
        return (pos == 0) ? (seq_t)2 : (seq_t)4;
    } else if (g == 7) {
        /* Dark Blue group: 35, 50 */
        return (pos == 0) ? (seq_t)35 : (seq_t)50;
    } else {
        /* Groups with 3 properties: rent pattern 6,6,8; 10,10,12; etc. */
        seq_t base = (seq_t)6 + (seq_t)4 * ((seq_t)(g - 1));
        return (pos == 2) ? base + (seq_t)2 : base;
    }
}
