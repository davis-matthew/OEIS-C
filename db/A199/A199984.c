/*
 * A199984  Composite numbers whose multiplicative digital root is 4.
 * https://oeis.org/A199984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   composite numbers whose multiplicative digital root is 4
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* product of decimal digits of x */
static inline seq_t digit_product(seq_t x)
{
    seq_t prod = 1;
    while (x > 0) {
        seq_t d = x % 10;
        prod *= d;
        x /= 10;
    }
    return prod;
}

/* true if x>3 and not prime (i.e. composite) */
static inline bool is_composite(seq_t x)
{
    if (x <= 3) return false;          /* 0,1,2,3 are not composite */
    if ((x & 1) == 0) return true;     /* even numbers >2 are composite */
    for (seq_t i = 3; i <= x / i; i += 2) {
        if (x % i == 0) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset 1; this case unused */
    idx_t count = 0;
    seq_t num = 2;
    while (1) {
        if (is_composite(num)) {
            seq_t mdr = num;
            do {
                mdr = digit_product(mdr);
            } while (mdr >= 10);
            if (mdr == 4) {
                ++count;
                if (count == n) return num;
            }
        }
        ++num;
    }
}
