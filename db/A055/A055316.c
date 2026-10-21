/*
 * A055316  Number of labeled trees with n nodes and 4 leaves.
 * https://oeis.org/A055316
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-4)*(n-3)*(n-2)*(3*n-11) / 576, n ≥ 5
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the expression using multiplicative cancellation of the
   2^6 and 3^2 factors of 576 so that intermediate results stay
   within the range of seq_t for the 64‑bit build, and work with
   arbitrary‑precision seq_t in other builds. */

seq_t a(idx_t n)
{
    if (n < 5) return 0;           /* values below the offset are irrelevant */

    /* We need to cancel 2^6 * 3^2 = 576 from the product.
       We'll remove 2s and 3s from the factors as we multiply. */
    int need2 = 6;
    int need3 = 2;

    seq_t res = 1;

    /* Multiply all integers from 2 to n, cancelling 2s and 3s on the fly */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t x = (seq_t)i;

        /* cancel factors of 2 */
        while (need2 > 0 && (x % 2) == 0) {
            x /= 2;
            --need2;
        }

        /* cancel factors of 3 */
        while (need3 > 0 && (x % 3) == 0) {
            x /= 3;
            --need3;
        }

        res *= x;
    }

    /* Compute the polynomial part: (n-4)*(n-3)*(n-2)*(3*n-11) */
    seq_t poly = (seq_t)(n - 4) *
                 (seq_t)(n - 3) *
                 (seq_t)(n - 2) *
                 (seq_t)(3 * n - 11);

    /* Cancel remaining 2s and 3s from the polynomial factor */
    while (need2 > 0 && (poly % 2) == 0) {
        poly /= 2;
        --need2;
    }
    while (need3 > 0 && (poly % 3) == 0) {
        poly /= 3;
        --need3;
    }

    /* Multiply the remaining polynomial part */
    res *= poly;

    /* If any 2s or 3s are still remaining, divide them out */
    while (need2 > 0) {
        res /= 2;
        --need2;
    }
    while (need3 > 0) {
        res /= 3;
        --need3;
    }

    return res;
}
