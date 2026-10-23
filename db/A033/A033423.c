/*
 * A033423  a(n) = floor(10^9/n).
 * https://oeis.org/A033423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(10^9 / n)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  A zero index is handled
       defensively to avoid a division‑by‑zero crash. */
    if (n == 0)
        return 0;

    const seq_t ONE_BILLION = (seq_t)1000000000;
    return ONE_BILLION / (seq_t)n;
}
