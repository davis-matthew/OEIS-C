/*
 * A028729  Nonsquares mod 16.
 * https://oeis.org/A028729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028729 – Nonsquares mod 16
 *
 * The set of quadratic residues modulo 16 is {0,1,4,9}.  The
 * remaining residues are the nonsquares:
 *   {2,3,5,6,7,8,10,11,12,13,14,15}
 * There are 12 such residues, so the sequence repeats every 16
 * numbers.  For a given index n (offset 1) we compute:
 *
 *   block = floor((n-1)/12)
 *   rem   = (n-1) mod 12
 *   residue = rem + 2,  but +1 if the value is ≥4, and +1 if it is ≥9
 *
 *   a(n) = block*16 + residue
 *
 * This uses only integer arithmetic and works for any integer
 * types seq_t and idx_t supplied by the build.  No lookup tables
 * of sequence values are used.  The implementation is O(1) and
 * meets the 2‑second per call requirement for n up to 10 000. */
seq_t a(idx_t n)
{
    if (n == 0)                /* below the offset – behaviour is
                                 unspecified but here we return 0 */
        return 0;

    /* zero‑based index of the desired element */
    idx_t idx   = n - 1;
    idx_t block = idx / 12;     /* block of 16 that precedes the term */
    idx_t rem   = idx % 12;     /* position within that block */

    /* compute the residue (the (rem)-th nonsquare modulo 16) */
    idx_t res = rem + 2;        /* base: 2 .. 13 */
    if (res >= 4)               /* skip 4 */
        ++res;
    if (res >= 9)               /* skip 9 */
        ++res;

    /* combine block and residue to get the nth term */
    return (seq_t)block * 16 + (seq_t)res;
}
