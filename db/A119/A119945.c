/*
 * A119945  Sum of numerical values of letters in German names of the
 *          nonnegative numbers.
 * https://oeis.org/A119945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, word
 * Formula:   a(n) = sum of letter values of the German name of the
 *            nonnegative integer n
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* pre‑computed sums of the letter values (a=1 … z=26) for the basic words */
static const seq_t UNIT_SUMS[20] = {59,47,63,36,54,52,54,54,32,54,53,23,87,89,107,105,88,88,85,107};
static const seq_t TENS_SUMS[10] = {0,0,106,90,96,94,77,77,74,96};

/* sums of tokens that are used repeatedly */
static const seq_t SUM_UND   = 39;   /* "und"      */
static const seq_t SUM_HUNDERT = 90; /* "hundert"  */
static const seq_t SUM_TAUSEND = 84; /* "tausend"  */
static const seq_t SUM_EIN   = 28;   /* "ein"      */

/* recursive routine that returns the sum of letter values for the German name of n */
static seq_t name_sum(idx_t n)
{
    /* special case 0 */
    if (n == 0) return UNIT_SUMS[0];

    /* 1 … 19 */
    if (n < 20) return UNIT_SUMS[n];

    /* 20 … 99 */
    if (n < 100) {
        idx_t tens  = n / 10;
        idx_t unit  = n % 10;
        if (unit == 0)
            return TENS_SUMS[tens];
        return (unit == 1 ? SUM_EIN : UNIT_SUMS[unit]) + SUM_UND + TENS_SUMS[tens];
    }

    /* 100 … 999 */
    if (n < 1000) {
        idx_t hundreds = n / 100;
        idx_t rem       = n % 100;
        seq_t s = (hundreds == 1 ? SUM_HUNDERT : UNIT_SUMS[hundreds] + SUM_HUNDERT);
        if (rem) s += name_sum(rem);
        return s;
    }

    /* 1000 … 999999 (handled up to the required range of 10000) */
    idx_t thousands = n / 1000;
    idx_t rem       = n % 1000;
    seq_t s = (thousands == 1 ? SUM_TAUSEND : name_sum(thousands) + SUM_TAUSEND);
    if (rem) s += name_sum(rem);
    return s;
}

/* required public function */
seq_t a(idx_t n)
{
    return name_sum(n);
}
