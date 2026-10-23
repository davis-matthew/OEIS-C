/*
 * A047484  Numbers that are congruent to {3, 5, 7} mod 8.
 * https://oeis.org/A047484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(3k-2) = 8k-5; a(3k-1) = 8k-3; a(3k) = 8k-1 where k =
 *            floor((n+2)/3). This gives all numbers ≡ 3,5,7 (mod 8)
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n<1 is undefined. */
    seq_t k = ((seq_t)n + 2) / 3;          /* floor((n+2)/3)  */
    idx_t r = n % 3;                      /* 0,1,2 correspond to n≡0,1,2 (mod 3) */
    switch (r)
    {
        case 1:  return k * 8 - 5;  /* a(3k-2) */
        case 2:  return k * 8 - 3;  /* a(3k-1) */
        default: return k * 8 - 1;  /* a(3k)   */
    }
}
