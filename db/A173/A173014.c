/*
 * A173014  a(1) = 1, for n >= 2; a(n) = the smallest number h such that
 *          sigma(h) = A000203(h) = a(n-1) + 4, a(n) = 0 if no such number
 *          exists.
 * https://oeis.org/A173014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n≥2, a(n) is periodic with period (0,3,4,7)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    idx_t r = (n - 2) % 4;   /* n≥2 ensures n-2 is non‑negative */
    switch (r) {
        case 0: return 0;
        case 1: return 3;
        case 2: return 4;
        case 3: return 7;
    }
    return 0; /* unreachable */
}
