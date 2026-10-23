/*
 * A169430  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*∑_{k=1}^{31} a(n-k) – 496*a(n-32) + b(n), where
 *            b(0)=1, b(1..31)=2, b(32)=1, b(n)=0 for n>32. This follows from
 *            the generating function
 *            (1+2∑_{k=1}^{31}x^k+x^32)/(1–31∑_{k=1}^{31}x^k+496x^32)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Circular buffer holding the last 33 terms a[n-32]…a[n]. */
    static seq_t buf[33];
    /* Re‑initialise buffer on each call (safe for any call order). */
    for (idx_t i = 0; i < 33; ++i)
        buf[i] = 0;

    for (idx_t i = 0; i <= n; ++i) {
        /* b(i) from the numerator of the GF */
        seq_t b;
        if (i == 0)           b = (seq_t)1;
        else if (i <= 31)     b = (seq_t)2;
        else if (i == 32)     b = (seq_t)1;
        else                  b = (seq_t)0;

        /* Sum of the last 31 terms, treating missing terms as 0. */
        seq_t sum = 0;
        for (idx_t k = 1; k <= 31 && k <= i; ++k)
            sum += buf[(i - k) % 33];

        /* Apply the linear recurrence. */
        seq_t term = (seq_t)31 * sum;
        seq_t sub  = 0;
        if (i >= 32)
            sub = (seq_t)496 * buf[(i - 32) % 33];

        seq_t a_n = term - sub + b;
        buf[i % 33] = a_n;
    }
    return buf[n % 33];
}
