/*
 * A018703  Divisors of 888.
 * https://oeis.org/A018703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 888 (sorted ascending)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    static seq_t divs[16];
    static int init = 0;
    if (!init) {
        const seq_t primes[3] = {(seq_t)2, (seq_t)3, (seq_t)37};
        const int exps[3]   = {3, 1, 1};
        int pos = 0;
        for (int e0 = 0; e0 <= exps[0]; ++e0)
            for (int e1 = 0; e1 <= exps[1]; ++e1)
                for (int e2 = 0; e2 <= exps[2]; ++e2)
                {
                    seq_t v = (seq_t)1;
                    for (int k = 0; k < e0; ++k) v *= primes[0];
                    for (int k = 0; k < e1; ++k) v *= primes[1];
                    for (int k = 0; k < e2; ++k) v *= primes[2];
                    divs[pos++] = v;
                }
        /* simple insertion sort */
        for (int i = 1; i < 16; ++i)
            for (int j = i - 1; j >= 0 && divs[j] > divs[j + 1]; --j) {
                seq_t t   = divs[j];
                divs[j]   = divs[j + 1];
                divs[j + 1] = t;
            }
        init = 1;
    }
    if (n < 1 || n > 16) return 0;
    return divs[n - 1];
}
