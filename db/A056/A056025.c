/*
 * A056025  Numbers k such that k^12 == 1 (mod 13^2).
 * https://oeis.org/A056025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 4652, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A056025 - Numbers k such that k^12 ≡ 1 (mod 13²).
   Each block of 12 consecutive terms is obtained from the previous
   block by adding 169.  The first 12 terms are fixed:
   a(1)=1, a(2)=19, a(3)=22, a(4)=23, a(5)=70, a(6)=80,
   a(7)=89, a(8)=99, a(9)=146, a(10)=147, a(11)=150, a(12)=168.
   Formula used:  a(n) = a(n-12) + 169  (n≥13)  or
   equivalently  a(n) = base[(n-1)%12] + 169 * ((n-1)/12). */

seq_t a(idx_t n)
{
    /* 1‑based base values for a(1)…a(12).  base[0] is unused. */
    static const seq_t base[13] = {
        0,              /* dummy to keep index 1‑based */
        1, 19, 22, 23, 70, 80, 89, 99, 146, 147, 150, 168
    };
    /* Position inside the 12‑term block (1 ≤ idx ≤ 12). */
    idx_t idx = ((n - 1) % 12) + 1;
    /* Number of complete 12‑term blocks preceding n. */
    idx_t q = (n - 1) / 12;
    return base[idx] + (seq_t)169 * q;
}
