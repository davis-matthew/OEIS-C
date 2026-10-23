/*
 * A047394  Numbers that are congruent to {0, 1, 6} mod 8.
 * https://oeis.org/A047394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047394 – Numbers congruent to {0,1,6} mod 8.
   The recurrence a(n) = a(n-1) + a(n-3) - a(n-4) for n>4
   is equivalent to the explicit formulas:
       if n % 3 == 0   a(n) = 8*(n/3)   - 2
       if n % 3 == 1   a(n) = 8*((n+2)/3) - 8
       if n % 3 == 2   a(n) = 8*((n+1)/3) - 7
   This implementation uses only integer arithmetic and works
   for any unsigned integer type seq_t and idx_t. */
seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; behaviour for n == 0 is undefined */
        return 0;

    idx_t r = n % 3;
    idx_t k;
    seq_t result;

    if (r == 0) {
        k = n / 3;
        result = (seq_t)8 * (seq_t)k - (seq_t)2;
    } else if (r == 1) {
        k = (n + 2) / 3;
        result = (seq_t)8 * (seq_t)k - (seq_t)8;
    } else { /* r == 2 */
        k = (n + 1) / 3;
        result = (seq_t)8 * (seq_t)k - (seq_t)7;
    }
    return result;
}
