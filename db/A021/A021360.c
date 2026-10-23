/*
 * A021360  Decimal expansion of 1/356.
 * https://oeis.org/A021360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * (10^n mod 356) / 356)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth digit after the decimal point in the expansion
   of 1/356 (OEIS A021360).  The digit is in the range 0..9, so
   it fits in any unsigned integer or big integer type used for seq_t. */
seq_t a(idx_t n)
{
    /* mod is small (356), so all calculations fit in 64‑bit integers */
    uint64_t mod = 356;
    uint64_t base = 10;
    uint64_t exp  = (uint64_t)n;
    uint64_t r = 1;          /* r = 10^exp mod mod */

    while (exp > 0) {
        if (exp & 1) r = (r * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    uint64_t digit = (10 * r) / mod;   /* 0 <= digit <= 9 */
    return (seq_t)digit;
}
