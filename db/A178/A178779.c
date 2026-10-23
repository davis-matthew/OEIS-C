/*
 * A178779  Expansion of the polynomial x^12*(x^6-1)*(x^4-1)*(x^3-1)*(x-1) in
 *          increasing powers of x.
 * https://oeis.org/A178779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full, easy
 * Formula:   * a(n) = coefficient of x^n in *
 *            x^12*(x^6-1)*(x^4-1)*(x^3-1)*(x-1) * This is computed by summing
 *            over all subsets of {6,4,3,1}: * a(n) = Σ_{S⊆{6,4,3,1}, ΣS =
 *            n-12} (-1)^{|S|} if 12≤n≤26, * otherwise a(n)=0
 * Range:     i8 n <= 26, i16 n <= 26, i32 n <= 26, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The polynomial has nonzero coefficients only for 12 ≤ n ≤ 26. */
    if (n < 12 || n > 26)
        return 0;

    /* n-12 is the target subset sum. */
    idx_t e = n - 12;

    /* Possible subset elements. */
    const int exp[4] = {6, 4, 3, 1};

    seq_t sign = 0;

    /* Enumerate all 2⁴ subsets. */
    for (int mask = 0; mask < 16; ++mask) {
        int sum = 0;
        int bits = 0;  /* size of the subset */
        /* Compute the sum of selected exponents. */
        if (mask & 1) { sum += exp[0]; ++bits; }
        if (mask & 2) { sum += exp[1]; ++bits; }
        if (mask & 4) { sum += exp[2]; ++bits; }
        if (mask & 8) { sum += exp[3]; ++bits; }

        if ((idx_t)sum == e) {
            /* Add contribution (-1)^bits. */
            sign += (bits & 1) ? (-1) : 1;
        }
    }

    return sign;
}
