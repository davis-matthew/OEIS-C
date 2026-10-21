/*
 * A099563  a(0) = 0; for n > 0, a(n) = final nonzero number in the sequence
 *          n, f(n,2), f(f(n,2),3), f(f(f(n,2),3),4),..., where f(n,d) =
 *          floor(n/d); the most significant digit in the factorial base
 *          representation of n.
 * https://oeis.org/A099563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / (2·3·…·k)) where k is the largest integer such
 *            that 2·3·…·k ≤ n. This equals the most significant digit in the
 *            factorial base representation of n. a(0) = 0
 * Range:     u8 n <= 9398, u16 n <= 9398, u32 n <= 9398, u64 n <= 9398,
 *            u128 n <= 9398, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Use 128‑bit product to avoid overflow for larger n. */
    __int128 prod = 1;
    for (int d = 2; ; ++d) {
        prod *= d;
        if (prod > ( __int128) n) {
            prod /= d;          /* restore product to 2·3·…·(d‑1) */
            break;
        }
    }
    /* prod now equals 2·3·…·k  with  prod ≤ n.  The digit is floor(n/prod). */
    return (seq_t)((__int128)n / prod);
}
