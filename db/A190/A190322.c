/*
 * A190322  a(1) = 1, a(2) = 9, a(3) = 17; for n>3, a(n) = a(n-3) + 2.
 * https://oeis.org/A190322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + 2 with a(1)=1, a(2)=9, a(3)=17
 * Range:     u8 n <= 362, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not in sequence, safe guard */
    idx_t k = (n - 1) / 3;            /* number of full 3‑steps */
    seq_t term = ((seq_t)k) * 2;      /* 2 added per step   */
    switch (n % 3) {                  /* remainder determines the base value */
        case 1: term += 1; break;
        case 2: term += 9; break;
        default: term += 17;           /* n % 3 == 0 */
    }
    return term;
}
