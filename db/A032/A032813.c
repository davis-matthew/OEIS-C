/*
 * A032813  Numbers whose set of base-13 digits is {2,3}.
 * https://oeis.org/A032813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the nth term of A032813: numbers whose base‑13 digits are {2,3}.
   For a given index n ≥ 1, the function finds the number by interpreting
   n-1 as a binary number that specifies which digits are 2 or 3.
   This is equivalent to enumerating all L‑digit base‑13 numbers composed
   only of 2 and 3 in increasing order. */
seq_t a(idx_t n)
{
    /* Determine the length L of the base‑13 representation. */
    idx_t target = n + 2;   /* target = n+2 */
    idx_t pow = 1;          /* pow = 2^k */
    idx_t len = 0;          /* number of shifts performed = k */

    while (pow < target) {
        pow <<= 1;
        ++len;
    }

    idx_t L = len - 1;                 /* L is the actual number of digits */
    idx_t prev = ((idx_t)1 << L) - 2;   /* count of numbers with fewer digits */

    idx_t offset = n - prev - 1;       /* offset within the current block */

    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i) {
        seq_t bit = (offset >> (L - 1 - i)) & 1;
        seq_t digit = 2 + bit;          /* digit is 2 or 3 */
        value = value * 13 + digit;
    }
    return value;
}
