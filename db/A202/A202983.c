/*
 * A202983  Number of nX3 0..2 arrays with every nonzero element less than or
 *          equal to some NW, E or S neighbor.
 * https://oeis.org/A202983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*a(n-1) -271*a(n-2) +1899*a(n-3) -11048*a(n-4) *
 *            +44346*a(n-5) -113218*a(n-6) +201165*a(n-7) -291398*a(n-8) *
 *            +360023*a(n-9) -334000*a(n-10) +199528*a(n-11) -66264*a(n-12) *
 *            +9212*a(n-13) * with a(1)=1, a(2)=63, a(3)=1233, a(4)=16854,
 *            a(5)=239196, * a(6)=3557168, a(7)=52328419, a(8)=7645...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: coef[i] corresponds to a(n-(i+1)) for i=0..12. */
static const int64_t coef[13] = {
    27LL,      -271LL,    1899LL,    -11048LL,   44346LL,
   -113218LL,  201165LL,  -291398LL,  360023LL,  -334000LL,
    199528LL,  -66264LL,    9212LL
};

static const seq_t init[13] = {
    (seq_t)1ULL,
    (seq_t)63ULL,
    (seq_t)1233ULL,
    (seq_t)16854ULL,
    (seq_t)239196ULL,
    (seq_t)3557168ULL,
    (seq_t)52328419ULL,
    (seq_t)764541713ULL,
    (seq_t)11194473148ULL,
    (seq_t)164061008324ULL,
    (seq_t)2403403907914ULL,
    (seq_t)35204470177631ULL,
    (seq_t)515704944067269ULL
};

seq_t a(idx_t n)
{
    /* offset starts at 1 */
    if (n == 0) return (seq_t)0;

    /* return known initial values */
    if (n <= 13) return init[n - 1];

    /* ring holds a(k-13) .. a(k-1); ring[12] is a(k-1) */
    seq_t ring[13];
    for (int i = 0; i < 13; ++i) ring[i] = init[i];

    for (idx_t k = 14; k <= n; ++k) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (int j = 0; j < 13; ++j) {
            const seq_t term = ring[12 - j];   /* a(k-(j+1)) */
            int64_t c = coef[j];
            if (c > 0) {
                pos += (seq_t)c * term;
            } else if (c < 0) {
                neg += (seq_t)(-c) * term;
            }
            /* c==0 cannot occur in this sequence */
        }
        seq_t newv = pos - neg;  /* pos >= neg due to positivity of a(k) */

        /* shift ring: drop oldest, and append newv */
        for (int i = 0; i < 12; ++i) ring[i] = ring[i + 1];
        ring[12] = newv;
    }

    return ring[12];
}
