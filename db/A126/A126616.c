/*
 * A126616  a(n) = n for n < 10, a(10*n) = a(n), and if the terms a(10),
 *          a(20), a(30), ... are deleted, one gets back the original
 *          sequence.
 * https://oeis.org/A126616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n for 1 ≤ n < 10, a(10·m) = a(m), a(n) = a(n –
 *            ⌊(n–1)/10⌋) for n % 10 ≠ 0 (self‑generating sequence with
 *            deleted 10ths) Offsets: a(1) is the first term
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    while (n >= 10u) {
        if ((n % 10u) == 0u) {       /* a(10·m) = a(m) */
            n /= 10u;
        } else {                      /* a(n) = a(n – ⌊(n‑1)/10⌋) */
            n -= (n - 1u) / 10u;
        }
    }
    return (seq_t)n;
}
