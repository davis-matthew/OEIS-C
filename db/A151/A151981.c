/*
 * A151981  Numbers k such that k^2 - k is divisible by 48.
 * https://oeis.org/A151981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A151981:
 * Numbers k such that k^2 - k is divisible by 48.
 * The terms are all integers congruent to 0, 1, 16, or 33 modulo 48,
 * sorted increasingly.
 *
 * Formula: let q = (n-1)/4, r = (n-1)%4; then
 *   r == 0 : a(n) = 48*q
 *   r == 1 : a(n) = 48*q + 1
 *   r == 2 : a(n) = 48*q + 16
 *   r == 3 : a(n) = 48*q + 33
 * Indexing starts at n = 1 (a(1) = 0).
 */

seq_t a(idx_t n)
{
    /* undefined for n < 1; return 0 as a safe placeholder */
    if (n == 0)
        return 0;

    /* 4‑term block computation */
    idx_t q = (n - 1) / 4;
    idx_t r = (n - 1) % 4;

    /* base = 48 * q as seq_t */
    seq_t base = (seq_t)48 * (seq_t)q;

    switch (r) {
        case 0: return base;               /* 48*q                */
        case 1: return base + (seq_t)1;    /* 48*q + 1            */
        case 2: return base + (seq_t)16;   /* 48*q + 16           */
        case 3: return base + (seq_t)33;   /* 48*q + 33           */
        default: return 0;                 /* never reached       */
    }
}
