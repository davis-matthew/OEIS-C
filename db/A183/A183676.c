/*
 * A183676  Number of (n+1) X 4 0..7 arrays with every 2 X 2 subblock summing
 *          to 14.
 * https://oeis.org/A183676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*8^n + 98*7^n + 300*6^n + 550*5^n + 776*4^n + 906*3^n +
 *            868*2^n + 590 (for n ≥ 1)
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The formula is valid for n≥1. For n==0 we return 0, although the sequence offset starts at 1. */
    if (n == 0)
        return 0;

    /* Compute the powers 2^n,3^n,…,8^n in a single linear loop. */
    seq_t pow2 = 1, pow3 = 1, pow4 = 1, pow5 = 1, pow6 = 1, pow7 = 1, pow8 = 1;

    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;
        pow3 *= 3;
        pow4 *= 4;
        pow5 *= 5;
        pow6 *= 6;
        pow7 *= 7;
        pow8 *= 8;
    }

    /* Combine with the integer coefficients. All coefficients fit in an ordinary unsigned integer. */
    seq_t result =
        (seq_t)8   * pow8 +
        (seq_t)98  * pow7 +
        (seq_t)300 * pow6 +
        (seq_t)550 * pow5 +
        (seq_t)776 * pow4 +
        (seq_t)906 * pow3 +
        (seq_t)868 * pow2 +
        (seq_t)590;

    return result;
}
