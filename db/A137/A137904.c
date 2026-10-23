/*
 * A137904  Rows 1, 3, 5, 7 of Mendeleyev-Seaborg (extended to 32 columns)
 *          periodic table elements.
 * https://oeis.org/A137904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = if n < 2: n+1 else if n < 10: 11 + (n-2) else if n < 28:
 *            37 + (n-10) else: 87 + ((n-28)/32)*64 + ((n-28)%32)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the OEIS sequence A137904: rows 1, 3, 5, 7 of the extended
   Mendeleyev-Seaborg (32‑column) periodic table. */
seq_t a(idx_t n)
{
    if (n < 2)            /* Period 1: 1, 2 */
        return (seq_t)n + 1;
    if (n < 10)           /* Period 3: 11–18 */
        return (seq_t)11 + ((seq_t)n - 2);
    if (n < 28)           /* Period 5: 37–54 */
        return (seq_t)37 + ((seq_t)n - 10);

    /* Periods 7, 9, 11, … (each 32 elements, separated by a skipped even
       period of 32 elements).  For n ≥ 28, compute offset within the
       current 32‑element block and adjust the start value. */
    idx_t rem     = n - 28;
    idx_t g       = rem / 32;   /* number of complete 32‑blocks past period 7 */
    idx_t offset  = rem % 32;   /* position within current 32‑block */

    seq_t base = (seq_t)87;           /* start of period 7   */
    seq_t add  = (seq_t)g * 64;       /* 2·32 per odd period beyond 7 */
    return base + add + (seq_t)offset;
}
