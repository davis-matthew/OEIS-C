/*
 * A237054  a(1)=1, a(n) = (a(n-1) + n) XOR n.
 * https://oeis.org/A237054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = (a(n-1) + n) XOR n
 * Range:     u8 n <= 20, u16 n <= 391, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* safety for out‑of‑range (not required) */

    seq_t val = 1;                /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        val = (val + (seq_t)i) ^ (seq_t)i;

    return val;
}
