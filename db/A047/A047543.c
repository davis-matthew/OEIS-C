/*
 * A047543  Numbers that are congruent to {3, 4, 7} mod 8.
 * https://oeis.org/A047543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A047543: Numbers congruent to 3, 4, 7 (mod 8)
    Formula:  let k = ceil(n/3) (i.e. k = (n+2)/3 for integer division).
    Then      a(3k-2) = 8k-5
              a(3k-1) = 8k-4
              a(3k)   = 8k-1
    which is equivalent to:
        r = (n-1) mod 3
        a(n) = 8*k - [5,4,1][r]   (index 0↦5, 1↦4, 2↦1)   */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. If called with n == 0,
       return 0 (undefined behaviour otherwise). */
    if (n == 0)
        return (seq_t)0;

    seq_t k = (seq_t)((n + 2) / 3);          /* ceil(n/3) */
    uint32_t r = (uint32_t)((n - 1) % 3);    /* 0,1,2 */

    /* 8*k is safely computed as a left shift. */
    seq_t prod = k << 3;                    /* prod = 8*k */

    switch (r) {
        case 0:  return prod - (seq_t)5;    /* a(3k-2) */
        case 1:  return prod - (seq_t)4;    /* a(3k-1) */
        default: return prod - (seq_t)1;    /* a(3k)   */
    }
}
