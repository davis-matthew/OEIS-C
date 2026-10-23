/*
 * A118857  Start with 1 and repeatedly place the first digit at the end of
 *          the number and add 12.
 * https://oeis.org/A118857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   for n > 1, let d be the decimal digit count of a(n-1),
   first = a(n-1) / 10^(d-1), rest = a(n-1) % 10^(d-1).
   Then a(n) = rest*10 + first + 12;  (move first digit to end and add 12) */
seq_t a(idx_t n)
{
    seq_t val = 1;
    if (n <= 1) return val;
    for (idx_t i = 2; i <= n; ++i) {
        /* find highest power of 10 <= val */
        seq_t pow10 = 1;
        while (pow10 * 10 <= val) {
            pow10 *= 10;
        }
        seq_t first = val / pow10;
        seq_t rest  = val % pow10;
        val = rest * 10 + first + 12;
    }
    return val;
}
