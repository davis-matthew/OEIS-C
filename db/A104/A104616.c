/*
 * A104616  Write the natural numbers in base 13 in a triangle with k digits
 *          in the k-th row, as shown below. Sequence gives the leading
 *          diagonal.
 * https://oeis.org/A104616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the digit at position n(n+1)/2 in the concatenation of
 *            the base‑13 representations of the natural numbers. This digit
 *            is the n‑th element on the leading diagonal of the triangle with
 *            k digits in row k
 * Range:     u8 n <= 21, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------------------------------------------------------- */
#include <stdint.h>

/* ----- helper: power of 13 ---------------------------------- */
static inline seq_t pow13(int d)
{
    seq_t r = 1;
    for (int i = 0; i < d; ++i) r *= 13;
    return r;
}

/* ----- helper: cumulative number of digits from 1 to x ---------- */
static seq_t cumulative_digits(seq_t x)
{
    if (x == 0) return 0;
    seq_t total = 0;
    int d = 1;
    seq_t lower = 1;          /* first number with d digits */
    seq_t upper = 12;         /* last number with 1 digit (12) */

    while (1) {
        seq_t count = (d == 1) ? 12 : 12 * pow13(d - 1); /* numbers of d digits */

        if (x <= upper) {                 /* x lies in current digit block */
            seq_t used = x - lower + 1;
            total += used * d;
            break;
        } else {                          /* take whole block */
            total += count * d;
            ++d;
            lower = pow13(d - 1);         /* 13^(d-1) */
            upper = pow13(d) - 1;
        }
    }
    return total;
}

/* ----- helper: binary search for smallest number with
          cumulative_digits >= target -------------------------- */
static idx_t find_number(seq_t target)
{
    idx_t low = 1, high = 1;
    /* grow high until it suffices */
    while (cumulative_digits(high) < target) {
        if (high > ((idx_t)-1) / 2) { /* prevent overflow */
            high = (idx_t)-1;
            break;
        }
        high <<= 1;
    }

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cumulative_digits(mid) < target)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* position (triangular number) */
    seq_t pos = (seq_t)n * ((seq_t)n + 1) / 2;

    /* find the number that contains this position */
    idx_t num = find_number(pos);
    seq_t prev = cumulative_digits((seq_t)(num - 1));
    idx_t idx_within = (idx_t)(pos - prev - 1);   /* 0‑based index inside the number */

    /* obtain the base‑13 digits of `num` */
    seq_t tmp = (seq_t)num;
    seq_t digits[64];            /* enough for many digits */
    int d = 0;
    while (tmp > 0) {
        digits[d++] = tmp % 13;
        tmp /= 13;
    }

    /* digits are stored LSB first; return the required one */
    return digits[d - 1 - (int)idx_within];
}
