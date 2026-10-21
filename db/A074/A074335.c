/*
 * A074335  In music, with 0 = C natural, 1 = C#, etc.: The unfolding of a
 *          semitonal interval cycle, alternating the ascending and descending
 *          aspects of the cycle from a common point or axis of symmetry. Any
 *          regularly occurring alignment may be used, with predictable even
 *          or odd results.
 * https://oeis.org/A074335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 0; For n >= 2 : Let d = n - 2. Let x = ((d >> 1) +
 *            1) mod 12. If d is even then a(n) = x; If d is odd then a(n) =
 *            (12 - x) mod 12
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074335: unfolding of the 12‑tone semitone cycle. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    idx_t d = n - 2;                     /* shift to start from 0 at n = 2   */
    unsigned long long x = (((unsigned long long)d >> 1) + 1) % 12ULL;
    unsigned long long res;

    if ((d & 1ULL) == 0)                 /* even index   */
        res = x;
    else                                 /* odd index    */
        res = (12ULL - x) % 12ULL;      /* wrap around modulo 12 */

    return (seq_t)res;
}
