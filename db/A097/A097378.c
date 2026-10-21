/*
 * A097378  a(n) = SquareFreeKernel(n)*CubeFreeKernel(n) + 1.
 * https://oeis.org/A097378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p^e‖n} p^(1+min(e,2)) + 1
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute the product of prime power factors as described. */
    idx_t m = n;          /* work on a copy, type idx_t is unsigned */
    seq_t prod = 1;

    /* Handle factor 2 separately for speed */
    if (m & 1ULL) {                /* if m is odd, no factor 2 */
    } else {
        uint64_t e = 0;
        while ((m & 1ULL) == 0) {   /* while m divisible by 2 */
            m >>= 1;
            e++;
        }
        if (e == 1)
            prod *= (seq_t)4;      /* 2^2 */
        else
            prod *= (seq_t)8;      /* 2^3, for e >= 2 */
    }

    /* Now factor odd primes */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            if (e == 1)
                prod *= (seq_t)p * (seq_t)p;          /* p^2 */
            else
                prod *= (seq_t)p * (seq_t)p * (seq_t)p; /* p^3 */
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1)
        prod *= (seq_t)m * (seq_t)m;  /* p^2 for single remaining prime */

    return prod + 1;
}
