/*
 * A018499  Divisors of 522.
 * https://oeis.org/A018499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th smallest divisor of 522
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 522 = 2^1 * 3^2 * 29^1, so we generate all 12 divisors. */
    seq_t divs[12];
    int k = 0;

    for (int a = 0; a <= 1; ++a) {
        seq_t va = (seq_t)1;
        for (int i = 0; i < a; ++i) va *= (seq_t)2;
        for (int b = 0; b <= 2; ++b) {
            seq_t vab = va;
            for (int i = 0; i < b; ++i) vab *= (seq_t)3;
            for (int c = 0; c <= 1; ++c) {
                seq_t v = vab;
                for (int i = 0; i < c; ++i) v *= (seq_t)29;
                divs[k++] = v;
            }
        }
    }

    /* Insertion sort to order the 12 divisors ascending. */
    for (int i = 1; i < 12; ++i) {
        seq_t key = divs[i];
        int j = i - 1;
        while (j >= 0 && divs[j] > key) {
            divs[j + 1] = divs[j];
            --j;
        }
        divs[j + 1] = key;
    }

    if (n >= 1 && n <= 12) {
        return divs[(int)(n - 1)];
    } else {
        /* Undefined for n > 12; return 0 as safe default. */
        return (seq_t)0;
    }
}
