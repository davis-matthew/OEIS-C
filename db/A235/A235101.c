/*
 * A235101  Number of (n+1) X (2+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) -14*a(n-2) -322*a(n-3) +949*a(n-4) +3384*a(n-5)
 *            -12901*a(n-6) -21740*a(n-7) +86236*a(n-8) +107744*a(n-9)
 *            -322761*a(n-10) -412538*a(n-11) +641088*a(n-12) +1018740*a(n-13)
 *            -478292*a(n-14) -1306432*a(n-15) -247248*a(n-16) +584736*a(n-17)
 *            +372096*a(n-18) +65664*a(n-19); initial value...
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const long long coeff[19] = {
    12, -14, -322, 949, 3384, -12901, -21740, 86236, 107744,
   -322761, -412538, 641088, 1018740, -478292, -1306432, -247248,
    584736,  372096,   65664
};

static const seq_t init[19] = {
    (seq_t)670ULL,          (seq_t)2498ULL,        (seq_t)9328ULL,        (seq_t)35354ULL,
    (seq_t)135254ULL,       (seq_t)523532ULL,      (seq_t)2047890ULL,     (seq_t)8097130ULL,
    (seq_t)32339728ULL,     (seq_t)130423938ULL,   (seq_t)530742334ULL,
    (seq_t)2177937580ULL,   (seq_t)9005319178ULL,  (seq_t)37492012258ULL,
    (seq_t)157045852768ULL, (seq_t)661404560282ULL, (seq_t)2798723937382ULL,
    (seq_t)11891816958860ULL, (seq_t)50708771987330ULL
};

seq_t a(idx_t n)
{
    if (n <= 19) {
        return init[n - 1];
    }

    /* arr[0] = a(cur-1), arr[1] = a(cur-2), … arr[18] = a(cur-19) */
    seq_t arr[19];
    for (int i = 0; i < 19; ++i) {
        arr[i] = init[18 - i];
    }

    seq_t result = 0;
    for (idx_t cur = 20; cur <= n; ++cur) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int i = 0; i < 19; ++i) {
            long long c = coeff[i];
            seq_t prod = arr[i] * (seq_t)(c >= 0 ? c : -c);
            if (c >= 0) {
                pos += prod;
            } else {
                neg += prod;
            }
        }
        result = pos - neg;                /* assuming pos >= neg for this recurrence */

        /* shift for next iteration: arr[i] becomes a(cur-i) */
        for (int i = 18; i > 0; --i) {
            arr[i] = arr[i - 1];
        }
        arr[0] = result;
    }

    return result;
}
