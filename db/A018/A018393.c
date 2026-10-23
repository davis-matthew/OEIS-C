/*
 * A018393  Divisors of 324.
 * https://oeis.org/A018393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 324 sorted in ascending order */

seq_t a(idx_t n)
{
    /* 324 = 2^2 * 3^4, so there are 15 divisors */
    const idx_t max_e2 = 2;
    const idx_t max_e3 = 4;
    seq_t vals[25];   /* sufficient to hold all divisors (15) */
    idx_t cnt = 0;

    /* generate all divisors by iterating over exponent combinations */
    seq_t pow2[3];
    pow2[0] = 1;
    for (idx_t i = 1; i <= max_e2; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;

    seq_t pow3[5];
    pow3[0] = 1;
    for (idx_t i = 1; i <= max_e3; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    for (idx_t e2 = 0; e2 <= max_e2; ++e2) {
        seq_t val2 = pow2[e2];
        for (idx_t e3 = 0; e3 <= max_e3; ++e3) {
            vals[cnt++] = val2 * pow3[e3];
        }
    }

    /* simple insertion sort (cnt is small = 15) */
    for (idx_t i = 1; i < cnt; ++i) {
        seq_t key = vals[i];
        idx_t j = i;
        while (j > 0 && vals[j - 1] > key) {
            vals[j] = vals[j - 1];
            --j;
        }
        vals[j] = key;
    }

    /* sequence indices start at 1 */
    if (n == 0 || n > cnt)
        return 0;   /* undefined for n outside the divisor list */
    return vals[n - 1];
}
