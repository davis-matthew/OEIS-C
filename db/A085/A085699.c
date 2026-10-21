/*
 * A085699  Smallest multiple of n such that a(n) + a(n-1) is an n-th power.
 * https://oeis.org/A085699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A085699, offset 1.
   a(1) = 1, a(2) = 8, a(3) = 117, a(4) = 508,
   a(5) = 32260, a(6) = 229884, a(7) = 19257287.
   For n > 7 the sequence has no defined term (no 8‑th power
   is congruent to a(7) modulo 8), so the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 8;
    if (n == 3) return 117;
    if (n == 4) return 508;
    if (n == 5) return 32260;
    if (n == 6) return 229884;
    if (n == 7) return 19257287;
    return 0;   /* undefined for n > 7 */
}
