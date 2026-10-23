/*
 * A140684  A037481 mod 10.
 * https://oeis.org/A140684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A037481(n) mod 10 Sequence is defined by the generating
 *            function G(x) = (7x^5 - 3x^4 - x^3 + 5x^2 + x) / (1 - x + x^5 -
 *            x^6) Hence the linear recurrence (for n ≥ 0) a(n) = b(n) +
 *            a(n-1) – a(n-5) + a(n-6), where b(n) = coeff of x^n in the
 *            numerator: 1,5,-1,-3,7 at n=1..5, otherwise b(n)=0....
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Buffer to hold the last 6 values a(n-1), a(n-2), … a(n-6) */
    seq_t buf[6] = {0};
    /* Special case n = 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    for (idx_t i = 0; i <= n; ++i) {
        /* Coefficient from the numerator polynomial */
        int coeffN = 0;
        switch (i) {
            case 1: coeffN =  1; break;
            case 2: coeffN =  5; break;
            case 3: coeffN = -1; break;
            case 4: coeffN = -3; break;
            case 5: coeffN =  7; break;
            default: coeffN =  0; break;
        }

        /* Compute the recurrence part:
           a(i) = coeffN + a(i-1) – a(i-5) + a(i-6)   (mod 10)   */
        int sum = 0;
        if (i >= 1)
            sum += -1 * (int)buf[(i - 1) % 6];   /* + a(i-1)  */
        if (i >= 5)
            sum +=  1 * (int)buf[(i - 5) % 6];   /* – a(i-5)  */
        if (i >= 6)
            sum += -1 * (int)buf[(i - 6) % 6];   /* + a(i-6)  */

        int val = coeffN - sum;                /* full value before %10 */
        int mod = val % 10;
        if (mod < 0) mod += 10;                /* ensure non‑negative */

        buf[i % 6] = (seq_t)mod;               /* store for future use */

        if (i == n)
            return (seq_t)mod;                 /* result found */
    }
    /* Should never reach here */
    return (seq_t)0;
}
