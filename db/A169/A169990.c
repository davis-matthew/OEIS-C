/*
 * A169990  Expansion of Product_{i=0..m-1} (1 + x^(2*i+1)) for m=7.
 * https://oeis.org/A169990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169990: coefficient of x^n in ∏_{i=0}^{6} (1 + x^{2*i+1}) */
seq_t a(idx_t n)
{
    /* static storage for the 50 coefficients of the polynomial */
    static int initialized = 0;
    static seq_t coeff[50];

    if (!initialized) {
        /* zero all entries */
        for (int i = 0; i < 50; ++i) coeff[i] = 0;
        coeff[0] = 1;

        const int exponents[7] = {1, 3, 5, 7, 9, 11, 13};
        int current_max = 0;

        for (int e = 0; e < 7; ++e) {
            int val = exponents[e];
            /* update coefficients in reverse order to avoid overwriting */
            for (int j = current_max; j >= 0; --j) {
                seq_t add = coeff[j];
                if (add) coeff[j + val] += add;
            }
            current_max += val;
        }
        initialized = 1;
    }

    if (n < 50) return coeff[n];
    return 0;
}
