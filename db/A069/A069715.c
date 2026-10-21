/*
 * A069715  GCD of digits of n is 1.
 * https://oeis.org/A069715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 198, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th positive integer whose decimal digit GCD is 1.
   Computed by scanning integers in order and selecting those whose digits
   have a greatest common divisor equal to 1. */

static int gcd_int(int a, int b) {
    while (b) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* guard against non‑positive n (not used in tests) */
    if (n == 0) return (seq_t)0;

    idx_t current = 1;      /* integer value to test   */
    idx_t count   = 0;      /* how many valid numbers seen */

    for (;;) {
        /* compute GCD of the decimal digits of current */
        idx_t x = current;
        int g = 0;          /* 0 means "no non‑zero digit seen yet" */
        while (x > 0) {
            int digit = x % 10;
            x /= 10;
            if (digit == 0) continue;          /* 0 doesn't change the GCD */
            if (g == 0)
                g = digit;
            else
                g = gcd_int(g, digit);
        }

        /* If all digits were zero (cannot happen for current>=1),
           we would have g==0; treat it as non‑valid. */
        if (g == 1) {
            count++;
            if (count == n)
                return (seq_t)current;
        }
        current++;               /* move to the next integer */
    }
}
