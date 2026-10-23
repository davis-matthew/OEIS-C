/*
 * A202625  Order of Fibonacci group F(n,3) (or 0 if the group is infinite).
 * https://oeis.org/A202625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n‑1 if 3 | n, a(n) = n^3 – 1 if n ≡ 1 (mod 3), a(n) = 8
 *            if n = 2, a(n) = 0 otherwise (i.e., n ≡ 2 (mod 3) and n ≠ 2)
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote to seq_t for arithmetic.  */
    seq_t n_s = (seq_t)n;

    /* n ≡ 0 (mod 3) → finite order n‑1. */
    if (n % 3U == 0U)
    {
        /* For n = 0 it would underflow; however the sequence is defined
           only for n ≥ 1.  */
        return n_s - 1;
    }

    /* n ≡ 1 (mod 3) → finite order n³ – 1. */
    if (n % 3U == 1U)
    {
        seq_t n2 = n_s * n_s;   /* n²   */
        seq_t n3 = n2 * n_s;    /* n³   */
        return n3 - 1;
    }

    /* n ≡ 2 (mod 3) */
    if (n == 2U)
    {
        /* The only case with a non‑zero finite order.  */
        return 8;
    }

    /* All other n ≡ 2 (mod 3) correspond to infinite groups. */
    return 0;
}
