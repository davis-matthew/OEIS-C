/*
 * A138228  Atomic numbers at the top of the 18 groups of the
 *          Mendeleyev-Seaborg table A138724.
 * https://oeis.org/A138228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, less
 * Formula:   a(0)=1, a(1)=4, a(n)=21+n-2 for 2≤n≤11, a(12)=5, a(13)=6,
 *            a(14)=7, a(15)=8, a(16)=9, a(17)=2, and a(n)=0 for n≥18
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)4;
    if (n <= 11ULL)
        return (seq_t)(21 + (n - 2));
    if (n == 12ULL) return (seq_t)5;
    if (n == 13ULL) return (seq_t)6;
    if (n == 14ULL) return (seq_t)7;
    if (n == 15ULL) return (seq_t)8;
    if (n == 16ULL) return (seq_t)9;
    if (n == 17ULL) return (seq_t)2;
    /* The sequence is defined only for 0≤n≤17. */
    return (seq_t)0;
}
