/*
 * A160080  Lodumo_4 of Fibonacci numbers.
 * https://oeis.org/A160080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lod_4(Fib(n)) Equivalent periodic description (n = 6*k +
 *            r): k = n / 6, r = n % 6 a(6k) = 4k a(6k+1) = 12k + 1 a(6k+2) =
 *            12k + 5 a(6k+3) = 4k + 2 a(6k+4) = 4k + 3 a(6k+5) = 12k + 9
 *            (Recurrence: a(n) = 2*a(n-6) - a(n-12) for n >= 12)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 6;
    switch (n % 6) {
        case 0: return (seq_t)4 * (seq_t)k;
        case 1: return (seq_t)12 * (seq_t)k + 1;
        case 2: return (seq_t)12 * (seq_t)k + 5;
        case 3: return (seq_t)4 * (seq_t)k + 2;
        case 4: return (seq_t)4 * (seq_t)k + 3;
        default: /* case 5 */ return (seq_t)12 * (seq_t)k + 9;
    }
}
