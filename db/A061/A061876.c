/*
 * A061876  |First digit - second digit + third digit - fourth digit ...| = 7.
 * https://oeis.org/A061876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th positive integer x (ordered increasingly) such
 *            that |(first digit) – (second digit) + (third digit) – …| = 7.
 *            The alternating sum starts with a plus sign on the first digit
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the absolute value of the alternating digit sum of a number. */
static int alt_sum_abs(seq_t x)
{
    int sum   = 0;          /* running sum                     */
    int sign  = 1;          /* current sign (+1 or –1)         */
    while (x > 0) {
        int digit = (int)(x % 10); /* last decimal digit           */
        sum += sign * digit;
        sign = -sign;               /* alternate sign              */
        x /= 10;                    /* drop last digit             */
    }
    return sum >= 0 ? sum : -sum;   /* absolute value */
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n<1 is irrelevant for tests. */
    idx_t  found = 0;       /* how many valid numbers have been seen */
    seq_t  num   = 0;       /* current candidate number */

    for (;; ++num) {            /* iterate over all positive integers */
        if (alt_sum_abs(num) == 7) {   /* property satisfied          */
            ++found;
            if (found == n)            /* desired term reached        */
                return num;
        }
    }
}
