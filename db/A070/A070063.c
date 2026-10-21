/*
 * A070063  Decimal expansion of the Boltzmann constant k in the 2019 SI
 *          system in units J/K.
 * https://oeis.org/A070063
 *
 * Offset:    -22    (a(-22) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = the (n+22)-nd digit after the decimal point of
 *            1.380649×10⁻²³, where digits before the non‑zero block are 0.
 *            For n≥-22 the sequence is 1,3,8,0,6,4,9,0,0,0,
 * Range:     u8 n <= -16, u16 n <= -16, u32 n <= -16, u64 n <= -16,
 *            u128 n <= -16, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Convert the index n into the position within the known digit block.  The
   offset of the sequence is -22, so `idx = n + 22`.  If idx is within the
   range [0,6] the corresponding digit of 1380649 is returned;
   otherwise 0 is returned (all subsequent digits are zero). */
seq_t a(idx_t n)
{
    /* digits of the non‑zero part of the Boltzmann constant after decimal point */
    static const seq_t digits[] = { 1, 3, 8, 0, 6, 4, 9 };
    const int offset   = -22;      /* sequence offset */
    const int num_digs = sizeof(digits) / sizeof(digits[0]);

    /* Calculate position relative to the offset */
    int idx = (int)n - offset;   /* n + 22 */

    if (idx >= 0 && idx < num_digs)
        return digits[idx];
    return 0; /* all remaining digits after the block are 0 */
}
