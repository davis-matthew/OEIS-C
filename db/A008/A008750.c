/*
 * A008750  Expansion of (1+x^7)/((1-x)*(1-x^2)*(1-x^3)).
 * https://oeis.org/A008750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001399(n) + A001399(n−7) A001399(n) = ⌊(n² + 6n + 12) /
 *            12⌋ (Sequence: expansion of (1+x⁷)/((1‑x)(1‑x²)(1‑x³)))
 * Range:     u8 n <= 39, u16 n <= 627, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t part = (nn * nn + 6 * nn + 12) / 12;   /* A001399(n) */
    seq_t res  = part;

    if (n >= 7)
    {
        idx_t m  = n - 7;
        seq_t mm = (seq_t)m;
        res += (mm * mm + 6 * mm + 12) / 12;      /* A001399(n‑7) */
    }

    return res;
}
