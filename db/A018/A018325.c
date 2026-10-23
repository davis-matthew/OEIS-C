/*
 * A018325  Divisors of 188.
 * https://oeis.org/A018325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th divisor of 188 (divisors: 1, 2, 4, 47, 94, 188) */
seq_t a(idx_t n)
{
    if (n < 1 || n > 6)
        return 0;          /* value undefined beyond the 6 divisors */

    static seq_t divs[6];
    static int initialized = 0;

    if (!initialized) {
        int idx = 0;
        /* 188 = 2^2 * 47^1 */
        for (uint8_t e1 = 0; e1 <= 2; ++e1) {
            seq_t p1 = (seq_t)1 << e1;               /* 2^e1 */
            for (uint8_t e2 = 0; e2 <= 1; ++e2) {
                seq_t p2 = e2 ? (seq_t)47 : (seq_t)1; /* 47^e2 */
                divs[idx++] = p1 * p2;
            }
        }

        /* simple bubble sort – only 6 elements, cost negligible */
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5 - i; ++j) {
                if (divs[j] > divs[j + 1]) {
                    seq_t tmp = divs[j];
                    divs[j] = divs[j + 1];
                    divs[j + 1] = tmp;
                }
            }
        }

        initialized = 1;
    }

    return divs[(int)(n - 1)];
}
