/*
 * A123144  a(1) = 1, a(n) = a(n-1) if n = 1 (mod 3), otherwise n*a(n-1).
 * https://oeis.org/A123144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 1
 * a(n) = a(n-1)               if n ≡ 1 (mod 3)
 * a(n) = n * a(n-1)           otherwise
 */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined in OEIS, handle safely */
        return 0;
    seq_t val = 1;        /* a(1) = 1 */
    for (idx_t k = 2; k <= n; ++k) {
        if (k % 3 != 1)
            val *= (seq_t)k;
    }
    return val;
}
