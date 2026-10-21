/*
 * A078195  In the following triangle the n-th row contains n n-digit (or
 *          (n-1)-digit) numbers whose concatenation (with a 0 prefixed for
 *          (n-1)-digit numbers) gives a substring of the cyclic concatenation
 *          of 1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,...: 1; 12 34; 123
 *          456 789; 1234 5678 9012 3456; 12345 67...
 * https://oeis.org/A078195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ_{j=0}^{n-1} value((j*n) mod 10 , n) where value(s,n) is
 *            the decimal number obtained from the n consecutive digits of the
 *            cyclic digit string 1,2,3,4,5,6,7,8,9,0 starting at offset s (0
 *            ≤ s < 10)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* digits of the infinite cyclic sequence, indexed 0..9 */
static const unsigned char DIG[10] = {1,2,3,4,5,6,7,8,9,0};

/* compute the block value for a given starting offset (0‑9) and length n */
static seq_t block_value(idx_t n, int start)
{
    seq_t val = 0;
    for (idx_t t = 0; t < n; ++t) {
        unsigned char d = DIG[(start + t) % 10];
        val = val * 10 + (seq_t)d;
    }
    return val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* pre‑compute values for all possible starting offsets */
    seq_t block[10];
    for (int s = 0; s < 10; ++s)
        block[s] = block_value(n, s);

    /* sum over the n blocks; the starting offset advances by n (mod 10) each time */
    int step   = (int)(n % 10);
    int offset = 0;
    seq_t sum   = 0;

    for (idx_t i = 0; i < n; ++i) {
        sum += block[offset];
        offset += step;
        if (offset >= 10) offset -= 10;
    }
    return sum;
}
