/*
 * A037908  Number of i such that |d(i) - d(i-1)| = 1, where Sum_{i=0..m}
 *          d(i)*5^i is the base-5 representation of n.
 * https://oeis.org/A037908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of positions i ≥ 1 in the base‑5 expansion of n
 *            where |digit[i] - digit[i-1]| = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n;          /* remaining part of n to process */
    seq_t count = 0;      /* result to return */

    int prev_digit = -1;  /* sentinel for the first digit */

    while (t > 0) {
        int digit = (int)(t % 5);   /* current base‑5 digit (0‑4) */

        /* If this is not the first digit, check the absolute difference. */
        if (prev_digit != -1) {
            int diff = digit - prev_digit;
            if (diff == 1 || diff == -1) {
                count = count + 1;
            }
        }

        prev_digit = digit;   /* remember for next iteration */
        t = t / 5;            /* shift right by one base‑5 digit */
    }

    return count;
}
