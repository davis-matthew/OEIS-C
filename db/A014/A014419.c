/*
 * A014419  Write n in base of Catalan numbers, then interpret as if written
 *          in base 3.
 * https://oeis.org/A014419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n written in the mixed radix system with weights Catalan
 *            numbers C_{k+1} (C₀ = 1, C₁ = 1, C₂ = 2, …), then interpret the
 *            resulting digits as a base‑3 number
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: n = 0 */
    if (n == 0)
        return 0;

    /* Pre‑compute Catalan weights C_{k+1} that are <= n. */
    /* A static array is sufficient because the index of n is
       far below the point where Catalan numbers overflow 64 bits. */
    idx_t w[200];
    int wcnt = 0;           /* number of stored weights */
    idx_t C = 1;            /* current Catalan C_k  (C_0 = 1) */
    int k = 0;              /* current index for recurrence */

    while (1) {
        /* next_C = C * 2*(2*k + 1) / (k + 2)   (C_{k+1})   */
        unsigned __int128 t = (unsigned __int128)C * 2 * (2 * k + 1) / (k + 2);
        if (t > n)
            break;                /* we have all weights <= n */
        idx_t nextC = (idx_t)t;   /* safe cast because t <= n <= max idx_t */
        w[wcnt++] = nextC;
        C = nextC;
        ++k;
    }

    /* Greedy extraction of mixed‑radix digits from high to low. */
    idx_t rem = n;
    idx_t digits[200];
    for (int i = wcnt - 1; i >= 0; --i) {
        digits[i] = rem / w[i];
        rem -= digits[i] * w[i];
    }

    /* Convert the digit string to a base‑3 value. */
    seq_t result = 0;
    seq_t pow3   = 1;                 /* 3^0 initially */
    for (int i = 0; i < wcnt; ++i) {
        result += (seq_t)digits[i] * pow3;
        pow3 *= 3;
    }
    return result;
}
