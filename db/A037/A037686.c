/*
 * A037686  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,3,2.
 * https://oeis.org/A037686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Small helper to fetch the n‑th digit of the periodic sequence 1,0,3,2 */
static inline unsigned int _digit_of(idx_t pos)
{
    /* pos ≥ 1, so (pos-1) fits into idx_t without underflow */
    switch ((pos-1U) & 3U)   /* 4‑cycle: 0→1, 1→0, 2→3, 3→2 */
    {
        case 0U: return 1U;
        case 1U: return 0U;
        case 2U: return 3U;
        default: return 2U;
    }
}
/* Recurrence:
   a(1) = 1
   a(n) = a(n-1)*9 + d_n, where d_n = 1,0,3,2 repeating.
   For n ≥ 1.                                                     */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        val = val * 9 + (seq_t)_digit_of(i);
    }
    return val;
}
