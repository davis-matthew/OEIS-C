/*
 * A015546  Inverse of 1537th cyclotomic polynomial.
 * https://oeis.org/A015546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{k=1}^{min(n,1456)} Φ_{1537}[k] · a(n-k), where
 *            Φ_{1537}(x)=∑_{k=0}^{1456} Φ_{1537}[k]·x^k. The values of
 *            Φ_{1537} are obtained by dividing (x-1)(x^1537-1) by
 *            (x^29-1)(x^53-1). The sequence is periodic with period 1537
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* constants for the fixed 1537‑th cyclotomic polynomial */
    enum { N = 1537, P = 29, Q = 53 };
    enum {
        DEG_NUM = N + 1,           /* = 1538, highest degree of the numerator   */
        SIZE_NUM = DEG_NUM + 1,    /* 1539, array size for indices 0..1538      */
        DEG_DEN = P + Q,           /* = 82  , highest degree of the denominator   */
        SIZE_DEN = DEG_DEN + 1,    /* 83   , array size for indices 0..82         */
        DEG_C   = DEG_NUM - DEG_DEN,  /* = 1456, degree of Φ_{1537}                 */
        SIZE_C  = DEG_C + 1          /* 1457, array size for Φ_{1537} coefficients */
    };

    /* static cache of the first period */
    static seq_t cache[SIZE_NUM];   /* SIZE_NUM = 1539, we use first 1537 entries */
    static seq_t coeff[SIZE_C];      /* Φ_{1537} coefficients, 0..1456           */
    static int initialized = 0;

    if (!initialized) {
        /* compute Φ_{1537} via polynomial long division */
        seq_t num[SIZE_NUM] = {0};
        seq_t den[SIZE_DEN] = {0};
        seq_t work[SIZE_NUM] = {0};

        /* numerator: (x-1)(x^1537-1) = x^1538 - x^1537 - x + 1  */
        num[0]      = 1;
        num[1]      = -1;
        num[N]       = -1;
        num[N + 1]   = 1;

        /* denominator: (x^29-1)(x^53-1) = x^82 - x^29 - x^53 + 1  */
        den[0]      = 1;
        den[P]      = -1;
        den[Q]      = -1;
        den[P + Q]  = 1;

        /* copy numerator to work array */
        for (int i = 0; i < SIZE_NUM; ++i) work[i] = num[i];

        /* long division: work = den * coeff  (coeff is Φ_{1537}) */
        for (int k = DEG_NUM; k >= DEG_DEN; --k) {
            seq_t c = work[k];          /* leading coefficient to cancel */
            int qdeg = k - DEG_DEN;     /* degree of the quotient coefficient */
            coeff[qdeg] = c;
            for (int j = 0; j <= DEG_DEN; ++j) {
                work[qdeg + j] -= c * den[j];
            }
        }

        /* recurrence for 1 / Φ_{1537} */
        cache[0] = 1;               /* a(0) = 1 */
        for (int m = 1; m < N; ++m) {
            seq_t sum = 0;
            int lim = (m <= DEG_C) ? m : DEG_C;
            for (int k = 1; k <= lim; ++k) {
                sum += coeff[k] * cache[m - k];
            }
            cache[m] = -sum;
        }

        initialized = 1;
    }

    /* periodicity: a(n) = a(n mod 1537) */
    uint64_t mod = n % N;
    return cache[(int)mod];
}
