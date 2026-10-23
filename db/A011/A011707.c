/*
 * A011707  A binary m-sequence: expansion of reciprocal of
 *          x^8+x^7+x^6+x^4+x^2+x+1.
 * https://oeis.org/A011707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the coefficient of x^n in the binary power series
 *            1/(x^8+x^7+x^6+x^4+x^2+x+1). The m‑sequence satisfies the linear
 *            recurrence a(n+8) = a(n+7) + a(n+6) + a(n+4) + a(n+2) + a(n+1) +
 *            a(n) (mod 2). This recurrence yields a 17‑periodic sequence. We
 *            compute the first 17 terms once and then retu...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t cache[17];
    static int initialized = 0;

    if (!initialized) {
        seq_t arr[17];
        /* initial 8 terms */
        arr[0] = 0; arr[1] = 0; arr[2] = 0; arr[3] = 0;
        arr[4] = 0; arr[5] = 0; arr[6] = 0; arr[7] = 1;

        /* compute terms 8..16 using the recurrence */
        for (idx_t k = 8; k < 17; ++k) {
            arr[k] = arr[k-8] ^ arr[k-7] ^ arr[k-6] ^ arr[k-4] ^ arr[k-2] ^ arr[k-1];
        }

        for (idx_t i = 0; i < 17; ++i) {
            cache[i] = arr[i];
        }
        initialized = 1;
    }

    idx_t r = n % 17;
    return cache[r];
}
