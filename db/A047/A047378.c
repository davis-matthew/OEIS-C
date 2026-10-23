/*
 * A047378  Numbers that are congruent to {2, 4, 5} mod 7.
 * https://oeis.org/A047378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7·floor((n-1)/3) + { 2 if (n-1) mod 3 = 0, 4 if (n-1) mod
 *            3 = 1, 5 if (n-1) mod 3 = 2 } This lists the positive integers
 *            congruent to 2, 4 or 5 modulo 7
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    if (n == 0) return 0;               /* defensive; undefined for n=0 */

    /* Using idx_t arithmetic, cast to seq_t for the final result */
    idx_t q = (n - 1) / 3;               /* number of complete 7‑blocks */
    idx_t r = (n - 1) % 3;               /* position within the block */

    seq_t base = (seq_t)7 * (seq_t)q;    /* 7 * q in seq_t */

    if (r == 0)
        return base + (seq_t)2;          /* 2 mod 7 */
    else if (r == 1)
        return base + (seq_t)4;          /* 4 mod 7 */
    else
        return base + (seq_t)5;          /* 5 mod 7 */
}
