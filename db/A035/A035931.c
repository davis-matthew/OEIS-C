/*
 * A035931  Number of steps to reach 0 under "k->max product of two numbers
 *          whose concatenation is k".
 * https://oeis.org/A035931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 911, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of steps to reach 0 under
 * k → max{ a*b | a,b are the two parts of k after a split }. */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;          /* start value */
    seq_t steps = 0;

    while (1) {
        if (k == 0)                 /* reached 0 */
            break;

        /* count decimal digits of k */
        int digits = 0;
        seq_t tmp = k;
        do {
            tmp /= 10;
            digits++;
        } while (tmp > 0);

        if (digits <= 1) {          /* single–digit: max product is 0 */
            steps++;                /* step that brings it to 0 */
            break;
        }

        /* current power of 10 for the most significant split */
        seq_t pow = 1;
        for (int i = 1; i < digits; i++)
            pow *= 10;              /* pow = 10^(digits-1) */

        seq_t maxProd = 0;
        /* try all splits between digits */
        for (int i = 1; i < digits; i++) {
            seq_t left  = k / pow;
            seq_t right = k % pow;
            seq_t prod  = left * right;
            if (prod > maxProd)
                maxProd = prod;
            pow /= 10;              /* shift one digit to the right */
        }

        k = maxProd;                /* go to the next number */
        steps++;
    }

    return steps;
}
