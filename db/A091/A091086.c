/*
 * A091086  a(n) = A000975(n) mod 10.
 * https://oeis.org/A091086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 - (-1)^n - cos(Pi*n/2) - 2*sin(Pi*n/2) For integer n,
 *            this reduces to a period‑4 sequence: n≡0→0, n≡1→1, n≡2→2, n≡3→5
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n mod 4 using bitwise AND (fast and works for all unsigned idx_t). */
    idx_t r = n & 3;              /* r ∈ {0,1,2,3} */
    /* Direct mapping to the period‑4 values */
    switch (r) {
        case 0: return 0;
        case 1: return 1;
        case 2: return 2;
        case 3: return 5;       /* constant fits in any seq_t */
        default: return 0;     /* unreachable, but keeps compiler happy */
    }
}
