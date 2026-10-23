/*
 * A018390  Divisors of 318.
 * https://oeis.org/A018390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th divisor of 318 in increasing order. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    static seq_t divs[8];
    static int init = 0;
    if (!init) {
        const unsigned long long primes[3] = {2ULL, 3ULL, 53ULL};
        int i;
        for (i = 0; i < 8; ++i) {
            seq_t val = 1;
            if (i & 1) val *= (seq_t)primes[0];
            if (i & 2) val *= (seq_t)primes[1];
            if (i & 4) val *= (seq_t)primes[2];
            divs[i] = val;
        }
        /* sort ascending */
        for (i = 0; i < 7; ++i) {
            int j;
            for (j = i + 1; j < 8; ++j) {
                if (divs[i] > divs[j]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }
        init = 1;
    }
    if (n <= 8) return divs[n - 1];
    return 0;
}
