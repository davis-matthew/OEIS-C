/*
 * A174984  Start with n, iterate the map k -> A167507(k) until we reach 3;
 *          a(n) = number of steps required.
 * https://oeis.org/A174984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = 1 if n≡4 (mod 10), 2 if n≡7, 3 if n≡5, 5 if n≡2, and 4
 *            otherwise
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t digit = n % 10;
    switch (digit) {
        case 4: return (seq_t)1;
        case 7: return (seq_t)2;
        case 5: return (seq_t)3;
        case 2: return (seq_t)5;
        default: return (seq_t)4;
    }
}
