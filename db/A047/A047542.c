/*
 * A047542  Numbers that are congruent to {0, 1, 2, 4, 7} mod 8.
 * https://oeis.org/A047542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5) + 8 for n > 5, with initial values a(1)=0, a(2)=1,
 *            a(3)=2, a(4)=4, a(5)=7. This implements the closed form a(n) =
 *            base[(n-1)%5] + 8 * ((n-1)/5). It works for all n ≥ 1 using only
 *            integer arithmetic
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for residues 0..4 (corresponding to r = (n-1)%5) */
    static const seq_t base[5] = {0, 1, 2, 4, 7};

    /* r = (n-1) % 5  -- 0,1,2,3,4 */
    idx_t r = (n - 1) % 5U;
    /* k = (n-1 - r) / 5  -- number of complete 5‑steps before index n */
    idx_t k = (n - 1 - r) / 5U;

    seq_t val = base[r];
    val += (seq_t)8 * (seq_t)k;
    return val;
}
