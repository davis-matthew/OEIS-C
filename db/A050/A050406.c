/*
 * A050406  Partial sums of A051880.
 * https://oeis.org/A050406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+5,5)*(5*n+3)/3 =
 *            (n+5)(n+4)(n+3)(n+2)(n+1)*(5*n+3)/360
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 79, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the intermediate product to avoid overflow
       on 64‑bit builds while keeping the final value in seq_t. */
    unsigned __int128 prod = 1;

    prod *= (unsigned __int128)(n + 1);
    prod *= (unsigned __int128)(n + 2);
    prod *= (unsigned __int128)(n + 3);
    prod *= (unsigned __int128)(n + 4);
    prod *= (unsigned __int128)(n + 5);

    /* multiply by (5*n + 3), cast before multiplication to avoid 64‑bit overflow */
    prod *= ((unsigned __int128)5 * n + 3);

    /* divide by 360 (120 * 3) */
    prod /= 360;

    return (seq_t)prod;   /* final result fits in seq_t for all n tested */
}
