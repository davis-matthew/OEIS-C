/*
 * A047244  Numbers that are congruent to {0, 2, 3} mod 6.
 * https://oeis.org/A047244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(ceil(n/3)-1) + {0,2,3} for n%3==1,2,0 respectively
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute ceil(n/3) as (n+2)/3 for unsigned integers */
    idx_t k = (n + 2) / 3;
    /* Base part 6*(ceil(n/3)-1) */
    idx_t base = 6 * (k - 1);

    switch (n % 3) {
        case 1: return (seq_t)base;          /* n ≡ 1 (mod 3) → 6k-6 */
        case 2: return (seq_t)(base + 2);     /* n ≡ 2 (mod 3) → 6k-4 */
        case 0: return (seq_t)(base + 3);     /* n ≡ 0 (mod 3) → 6k-3 */
        default: return (seq_t)0;            /* unreachable, safety */
    }
}
