/*
 * A138531  Decimal expansion of 109739369/111111111.
 * https://oeis.org/A138531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal digits of 109739369/111111111.
   The expansion repeats the 9‑digit pattern 9 8 7 6 5 4 3 2 1.
   Hence a(n) = 9 - (n mod 9). */
seq_t a(idx_t n)
{
    /* The digit cycle is 9–8–7–6–5–4–3–2–1. */
    return 9 - (seq_t)(n % 9);
}
