/*
 * A051811  Numbers with nonzero multiplicative digital root 9.
 * https://oeis.org/A051811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A051811 – Numbers with non‑zero multiplicative digital root 9 */
/* Compute a(n) by scanning natural numbers and returning the n‑th one whose MDR is 9. */

seq_t a(idx_t n) {
    seq_t current = 0;   /* start from 1 */
    idx_t count = 0;     /* how many terms found so far */

    while (count < n) {
        /* advance to next number */
        current += 1;

        /* compute multiplicative digital root of current */
        seq_t x = current;
        while (x >= 10) {
            seq_t prod = 1;
            seq_t temp = x;
            while (temp > 0) {
                seq_t d = temp % 10;
                if (d != 0) prod *= d;
                temp /= 10;
            }
            x = prod;
        }

        /* if the root is 9, keep it as a term */
        if (x == 9) {
            count += 1;
            if (count == n) return current;
        }
    }
    /* should never reach here */
    return 0;
}
