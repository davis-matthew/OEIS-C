/*
 * A018280  Divisors of 96.
 * https://oeis.org/A018280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   nth divisor of 96 in ascending order
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t N = (seq_t)96;          /* constant divisor base */
    seq_t divs[12];
    idx_t cnt = 0;

    /* Enumerate all divisors of 96 */
    for (idx_t d = 1; d * d <= (idx_t)N; ++d) {
        if (N % (seq_t)d == 0) {
            divs[cnt++] = (seq_t)d;
            if (d * d != (idx_t)N) {
                divs[cnt++] = N / (seq_t)d;
            }
        }
    }

    /* Simple bubble sort (12 elements, negligible cost) */
    for (idx_t i = 0; i < cnt; ++i) {
        for (idx_t j = i + 1; j < cnt; ++j) {
            if (divs[i] > divs[j]) {
                seq_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }
        }
    }

    if (n >= 1 && n <= cnt) {
        return divs[(idx_t)n - 1];
    }
    return (seq_t)0;
}
