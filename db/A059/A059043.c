/*
 * A059043  Numbers in which each digit is the (immediate) successor of the
 *          previous one (if it exists) and 0 is considered the successor of
 *          9.
 * https://oeis.org/A059043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 42, u32 n <= 85, u64 n <= 173,
 *            u128 n <= 345, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the n-th number whose decimal digits are successive modulo 10
   (no leading zeros except for the single digit 0).  For n ≤ 10,
   a(n) = n - 1.  For n > 10, the number has length
   L = ((n-11)/9) + 2 and begins with digit
   startDigit = (n-11)%9 + 1.  The value is built by appending digits
   (startDigit + i) mod 10 for i = 0 … L-1. */
seq_t a(idx_t n)
{
    if (n <= 10) {
        return (seq_t)(n - 1);
    }
    idx_t indexZero = n - 11;           // zero‑based index within groups of length ≥2
    idx_t L      = (indexZero / 9) + 2; // length of the desired number
    idx_t offset = indexZero % 9;       // offset within the group
    int startDigit = (int)offset + 1;   // starting digit (1..9)

    seq_t result = 0;
    for (idx_t i = 0; i < L; ++i) {
        int d = (startDigit + i) % 10;
        result = result * (seq_t)10 + (seq_t)d;
    }
    return result;
}
