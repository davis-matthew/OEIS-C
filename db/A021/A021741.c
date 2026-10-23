/*
 * A021741  Decimal expansion of 1/737.
 * https://oeis.org/A021741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021741: a(n) is the n‑th decimal digit of 1/737 (0‑indexed)
   Formula: a(n) = ⌊10^(n+1)/737⌋ mod 10, computed by long division. */
seq_t a(idx_t n)
{
    const idx_t PERIOD = 66;                 /* 1/737 has period 66 digits */
    idx_t k = n % PERIOD;                    /* print only within one period   */
    seq_t rem = 1;                           /* current remainder (≤736)       */
    seq_t digit = 0;
    for (idx_t i = 0; i <= k; ++i) {
        unsigned int prod = (unsigned int)rem * 10u;   /* rem*10 ≤ 7360    */
        digit = (seq_t)(prod / 737u);                 /* quotient digit   */
        rem   = (seq_t)(prod % 737u);                 /* update remainder */
    }
    return digit;
}
