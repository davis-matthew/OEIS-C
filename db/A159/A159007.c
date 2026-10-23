/*
 * A159007  Numbers k such that k == 32 or 41 (mod 73).
 * https://oeis.org/A159007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (73 + 55*(-1)^(n-1) + 146*(n-1)) / 4 (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 500, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined for n == 0, but return 0 for safety. */
    if (n == 0)
        return 0;

    /* Compute the numerator of the closed‑form expression. */
    seq_t num = 73 + 146 * ((seq_t)(n - 1));

    /* Add or subtract 55 depending on the parity of (n‑1). */
    if (((n - 1) & 1) == 0)          /* (n‑1) even → (+55) */
        num += 55;
    else                            /* (n‑1) odd  → (‑55) */
        num -= 55;

    /* Divide to obtain a(n).  The division is exact. */
    return num / 4;
}
