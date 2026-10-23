/*
 * A197498  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 2,3,1,1,0 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A197498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+2*a(n-2)+7*a(n-3)+15*a(n-4)+15*a(n-5)+6*a(n-6)
 *            -9*a(n-7)-14*a(n-8)-46*a(n-9)-63*a(n-10)+3*a(n-11)+a(n-12)+39*a(n-13)
 *            -16*a(n-14)+39*a(n-15)+27*a(n-16)+32*a(n-17)-3*a(n-18)-11*a(n-19)
 *            -14*a(n-20)-8*a(n-21) (for n ≥ 22) Initial values a(1)…a(21) are
 *            listed below
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* first 21 terms (indices 1–21) */
    static const seq_t init[21] = {
        (seq_t)1, (seq_t)9, (seq_t)22, (seq_t)53,
        (seq_t)162, (seq_t)526, (seq_t)1605, (seq_t)5011,
        (seq_t)15073, (seq_t)46562, (seq_t)143487, (seq_t)440836,
        (seq_t)1353988, (seq_t)4162099, (seq_t)12795741, (seq_t)39330623,
        (seq_t)120882310, (seq_t)371559669, (seq_t)1142099870,
        (seq_t)3510484802, (seq_t)10790188695
    };
    /* recurrence coefficients for a(n) in order a(n-1) … a(n-21) */
    static const int coeff[21] = {
        1,   2,    7,    15,   15,    6,  -9, -14, -46, -63,
        3,    1,   39,  -16,   39,   27,   32,  -3, -11, -14, -8
    };

    /* values for n ≤ 21 are known directly. */
    if (n <= 21)
        return init[n - 1];

    /* buffer holding a(n-1)…a(n-21) (most recent first). */
    seq_t last[21];
    /* initialise with a(21), a(20), …, a(1) */
    for (int i = 0; i < 21; ++i)
        last[i] = init[20 - i];

    /* iterate from n = 22 up to the requested n */
    idx_t cur;
    for (cur = 22; cur <= n; ++cur) {
        seq_t pos = 0, neg = 0;
        for (int j = 0; j < 21; ++j) {
            if (coeff[j] > 0)
                pos += last[j] * (unsigned long long)coeff[j];
            else
                neg += last[j] * (unsigned long long)(-coeff[j]);
        }
        seq_t new_term = pos - neg;   /* guaranteed non‑negative */

        /* shift buffer: drop a(n-21), move others, insert new_term */
        for (int i = 20; i > 0; --i)
            last[i] = last[i - 1];
        last[0] = new_term;
    }
    return last[0];
}
