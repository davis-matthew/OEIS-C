/*
 * A008544  Triple factorial numbers: Product_{k=0..n-1} (3*k+2).
 * https://oeis.org/A008544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⨍_{k=0..n-1} (3·k + 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following function computes the n‑th term of OEIS A008544:
   a(0) = 1, a(n) = a(n‑1) * (3·(n‑1) + 2) for n ≥ 1.
   It works for any seq_t (8–128 bit unsigned integer or a big‑int
   class) and for any idx_t that can hold the loop index. */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 0; i < n; ++i) {
        res *= (seq_t)(3 * i + 2);
    }
    return res;
}
