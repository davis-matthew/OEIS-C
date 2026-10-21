/*
 * A231004  Number of years after which it is not possible to have the same
 *          calendar for the entire year, in the Julian calendar.
 * https://oeis.org/A231004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28 * floor((n-1)/23) + r where r is the (n-1) mod 23‑th
 *            element of the cyclic list of "bad" years in a 28‑year block:
 *            [1,2,3,4,5,7,8,9,10,12,13,14,15,16,18,19,20,21,23,24,25,26,27]
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;        /* undefined for n = 0, conventionally return 0 */
    }
    /* number of full 28‑year blocks fully preceding the n‑th bad year */
    idx_t full_cycles = (n - 1) / 23;
    /* offset inside the current block (0‑based) */
    idx_t offset = (n - 1) % 23;

    /* residues of bad years within one 28‑year block */
    static const idx_t bad_residues[23] = {
        1, 2, 3, 4, 5,
        7, 8, 9,10,12,
       13,14,15,16,18,
       19,20,21,23,24,
       25,26,27
    };

    /* compute a(n) in seq_t type, avoiding overflow for 64‑bit builds */
    seq_t year = ((seq_t)full_cycles * (seq_t)28) +
                 (seq_t)bad_residues[offset];
    return year;
}
