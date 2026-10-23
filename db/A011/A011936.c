/*
 * A011936  a(n) = floor( n*(n-1)*(n-2)*(n-3)/26 ).
 * https://oeis.org/A011936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/26)
 * Range:     u8 n <= 10, u16 n <= 37, u32 n <= 579, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the product is negative or zero, floor gives 0 */
    if (n < 4) return 0;

    /* Use an unsigned 128‑bit intermediate to avoid overflow for 64‑bit
       and 32‑bit seq_t builds, while remaining compatible with the
       arbitrary‑precision build. */
    unsigned __int128 prod = (unsigned __int128)n * (n-1) * (n-2) * (n-3);
    prod /= 26;
    return (seq_t)prod;
}
