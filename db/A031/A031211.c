/*
 * A031211  Periods of sum of 8th powers of digits iterated until the sequence
 *          becomes periodic.
 * https://oeis.org/A031211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the period (cycle length) of the sequence obtained by 
   iterating f(x)=sum of 8th powers of decimal digits, starting from n.
   Uses Floyd's cycle‑finding algorithm. */
#include <stdint.h>

static const seq_t pow8[10] = {
    0ULL,          /* 0^8 */
    1ULL,          /* 1^8 */
    256ULL,        /* 2^8 */
    6561ULL,       /* 3^8 */
    65536ULL,      /* 4^8 */
    390625ULL,     /* 5^8 */
    1679616ULL,    /* 6^8 */
    5764801ULL,    /* 7^8 */
    16777216ULL,   /* 8^8 */
    43046721ULL    /* 9^8 */
};

/* Compute next value f(x) = sum of eighth powers of decimal digits of x. */
static seq_t next_value(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        seq_t d = x % (seq_t)10;
        sum += pow8[(unsigned)d];
        x /= (seq_t)10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t start = (seq_t)n;

    /* Floyd's cycle-finding algorithm */
    seq_t tortoise = next_value(start);
    seq_t hare = next_value(next_value(start));
    while (tortoise != hare) {
        tortoise = next_value(tortoise);
        hare = next_value(next_value(hare));
    }

    /* Find cycle length lambda */
    seq_t lam = 1;
    hare = next_value(tortoise);
    while (tortoise != hare) {
        hare = next_value(hare);
        lam++;
    }

    return lam;
}
