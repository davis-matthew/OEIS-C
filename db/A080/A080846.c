/*
 * A080846  Fixed point of the morphism 0->010, 1->011, starting from a(1) =
 *          0.
 * https://oeis.org/A080846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if (n-1) mod 3 == 0 1 if (n-1) mod 3 == 1 a((n-1-2)/3 +
 *            1) if (n-1) mod 3 == 2 (This is the recurrence a(3k)=0,
 *            a(3k+1)=1, a(3k-1)=a(k-1) with 0‑based indices, converted to
 *            1‑based.)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert to 0‑based index */
    idx_t i = n - 1;
    /* Handle the 3k-1 case by mapping down to a lower index */
    while (i % 3 == 2) {
        i = (i - 2) / 3;
    }
    /* i % 3 is now 0 or 1 */
    return (i % 3 == 0) ? 0 : 1;
}
