/*
 * A053209  Row sums of A051598.
 * https://oeis.org/A053209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n)=9*2^(n-1)-4 for n>=1 (equivalently, *
 *            a(n+1)=2*a(n)+4 with a(1)=5). This implementation uses the *
 *            linear recurrence, which is trivially O(n). It works for any *
 *            unsigned n, with a_n fitting into seq_t. For builds where *
 *            seq_t is 64‑bit, the sequence only fits up to n=61; larger *
 *            t...
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t val = (seq_t)5;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        val = 2 * val + 4;         /* a(i) = 2*a(i-1)+4 */
    }
    return val;
}
