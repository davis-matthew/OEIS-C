/*
 * A022441  a(n) = c(n) + c(n-1) where c (A055562) is the sequence of numbers
 *          not in a.
 * https://oeis.org/A022441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A022441
   a(0) = 1
   a(n) = b(n) for n ≥ 1
   b(0) = 0
   b(2*k)   = 9*k + 3   – b(k)
   b(2*k+1) = 9*k + 6   – b(k)   (with b(1) = 5)
   The recurrence yields all terms of the sequence. */
static seq_t b_rec(idx_t m)
{
    if (m == 0) return 0;                /* b(0) */
    if (m & 1) {                         /* m odd */
        idx_t k = (m - 1) >> 1;          /* k = (m-1)/2 */
        seq_t bk = b_rec(k);
        seq_t term = (seq_t)9 * k + (k == 0 ? 5 : 6);
        return term - bk;                /* term ≥ bk, unsigned arithmetic safe */
    } else {                             /* m even */
        idx_t k = m >> 1;                /* k = m/2 */
        seq_t bk = b_rec(k);
        seq_t term = (seq_t)9 * k + 3;
        return term - bk;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    return b_rec(n);
}
