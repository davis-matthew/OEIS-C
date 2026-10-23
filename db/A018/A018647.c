/*
 * A018647  Divisors of 788.
 * https://oeis.org/A018647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 788 in ascending order
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n=0 */

    static const seq_t N = (seq_t)788;
    static const int MAX_DIVS = 32;           /* plenty for 6 divisors */
    int divs[MAX_DIVS];
    int cnt = 0;

    /* Find all divisors of N */
    for (seq_t i = 1; i * i <= N; ++i) {      /* i is seq_t to avoid overflow */
        if (N % i == 0) {
            divs[cnt++] = (int)i;
            seq_t other = N / i;
            if (other != i && cnt < MAX_DIVS) {
                divs[cnt++] = (int)other;
            }
        }
    }

    /* Simple insertion (bubble) sort on the small array */
    for (int i = 0; i < cnt - 1; ++i) {
        for (int j = i + 1; j < cnt; ++j) {
            if (divs[i] > divs[j]) {
                int t = divs[i];
                divs[i] = divs[j];
                divs[j] = t;
            }
        }
    }

    if ((idx_t)(n - 1) < (idx_t)cnt) {
        return (seq_t)divs[n - 1];
    }
    return 0;                                 /* undefined for n>6 */
}
