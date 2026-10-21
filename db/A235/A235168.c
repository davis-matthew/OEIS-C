/*
 * A235168  Triangle read by rows: row n gives digits of n in primorial base.
 * https://oeis.org/A235168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base, look
 * Formula:   not stated
 * Range:     u8 n <= 9912, u16 n <= 9912, u32 n <= 9912, u64 n <= 9912,
 *            u128 n <= 9912, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235168: digits of non‑negative integers written in
   primorial base, read row by row.  The function returns the
   nth digit in this flattened table. */
#include "../utils/get_prime.h"

static idx_t primorial_len(idx_t x)
{
    if (x == 0) return 1;            // base case: row 0 has one digit 0
    idx_t len = 0;
    idx_t prime_index = 1;           // get_prime(k) uses 1‑based indexing
    while (x > 0) {
        x /= get_prime(prime_index);
        prime_index++;
        len++;
    }
    return len;
}

static seq_t primorial_digit(idx_t x, idx_t pos)
{
    if (x == 0) return 0;            // digit of row 0
    seq_t digits[64];                // enough for 64‑bit numbers
    idx_t len = 0;
    idx_t prime_index = 1;
    while (x > 0) {
        digits[len++] = x % get_prime(prime_index);
        x /= get_prime(prime_index);
        prime_index++;
    }
    /* digits[0] is least significant, digits[len‑1] most significant */
    return digits[len - 1 - pos];
}

seq_t a(idx_t n)
{
    idx_t total = 0;          // cumulative count of digits so far
    idx_t row = 0;
    while (1) {
        idx_t len = primorial_len(row);
        if (total + len > n) break;
        total += len;
        row++;
    }
    idx_t pos = n - total;    // position within the current row (0 = msb)
    return primorial_digit(row, pos);
}
