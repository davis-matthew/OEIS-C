/*
 * A080709  Take sum of squares of digits of previous term, starting with 4.
 * https://oeis.org/A080709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the orbit of 4 under repeated sum of squares of digits.
 *            The orbit is periodic with period 8: 4 → 16 → 37 → 58 → 89 → 145
 *            → 42 → 20 → … The function computes the first 8 values once and
 *            returns the term corresponding to (n − 1) mod 8
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t period[8];
static int period_initialized = 0;

static void init_period(void) {
    seq_t x = 4;
    for (int i = 0; i < 8; ++i) {
        period[i] = x;
        /* compute next term: sum of squares of digits of x */
        seq_t sum = 0;
        seq_t temp = x;
        while (temp > 0) {
            seq_t d = temp % 10;
            sum += d * d;
            temp /= 10;
        }
        x = sum;
    }
    period_initialized = 1;
}

seq_t a(idx_t n) {
    if (!period_initialized) {
        init_period();
    }
    /* (n-1) is safe because n >= 1 */
    return period[(int)((n - 1) % 8)];
}
