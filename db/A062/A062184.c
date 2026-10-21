/*
 * A062184  Harmonic mean of digits is 6.
 * https://oeis.org/A062184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth positive integer whose harmonic mean of its digits
 *            equals 6 (digits 1–9 only, no zeros)
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 6 * 2520 = 15120.  For a digit d, 6*2520/d is added to a running sum.
       The harmonic‑mean condition becomes: sum_{digits} (15120/d) = 2520 * #digits. */
    static const unsigned contrib[10] = {
        0,        /* 0 (unused) */
        15120,    /* 1  */
        7560,     /* 2  */
        5040,     /* 3  */
        3780,     /* 4  */
        3024,     /* 5  */
        2520,     /* 6  */
        2160,     /* 7  */
        1890,     /* 8  */
        1680      /* 9  */
    };

    unsigned long long found = 0;
    unsigned long long candidate = 6;   /* smallest admissible number */
    const unsigned long long target_mul = 2520ULL;

    for (;;) {
        unsigned long long x = candidate;
        unsigned sum = 0;
        unsigned len  = 0;

        while (x > 0) {
            unsigned d = (unsigned)(x % 10ULL);
            if (d == 0) {           /* zero digits are forbidden */
                sum = 0xffffffffU;   /* invalidate the sum */
                break;
            }
            sum += contrib[d];
            ++len;
            x /= 10ULL;
        }

        if (sum == target_mul * len) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
