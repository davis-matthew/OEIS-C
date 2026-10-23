/*
 * A025832  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^10)).
 * https://oeis.org/A025832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of partitions of n into parts 3, 4, and 10
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to counting non‑negative integer solutions of 3*a + 4*b + 10*c = n. */

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* iterate over possible numbers of 10’s */
    for (idx_t c = 0; c <= n / 10; ++c) {
        idx_t m = n - 10 * c;          /* remaining part after taking 10*c */
        idx_t max_b = m / 4;           /* largest b so that 4*b ≤ m */
        idx_t mod  = m % 3;            /* required congruence for b (b ≡ m (mod 3)) */
        if (mod > max_b) continue;     /* no b fits */
        idx_t first_b = mod;
        idx_t num_b = (max_b - first_b) / 3 + 1;  /* number of valid b’s */
        result += (seq_t)num_b;
    }
    return result;
}
