/*
 * A183795  1/4 the number of (n+1)X3 0..3 arrays with no 2X2 subblock having
 *          sum 6.
 * https://oeis.org/A183795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence of order 10
 *            a(n)=32*a(n-1)+611*a(n-2)-1696*a(n-3)-25173*a(n-4)+95128*a(n-5)+
 *            140335*a(n-6)-1227112*a(n-7)+2372316*a(n-8)-1955184*a(n-9)+601344*a(n-10)
 *            Initial values a(1) ... a(10) are given below. The sequence
 *            starts at index 1; values for n<1 are undefined
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 10‑term recurrence coefficients – see above formula */
static const long long coeffs[10] = {
    32,          /* +32  a(n-1) */
    611,         /* +611 a(n-2) */
   -1696,        /* -1696 a(n-3) */
   -25173,       /* -25173 a(n-4) */
    95128,       /* +95128 a(n-5) */
   140335,       /* +140335 a(n-6) */
  -1227112,      /* -1227112 a(n-7) */
   2372316,      /* +2372316 a(n-8) */
  -1955184,      /* -1955184 a(n-9) */
   601344       /* +601344 a(n-10) */
};

/* Initial terms a(1) … a(10).  Stored in natural order (index‑offset 1). */
static const seq_t init_vals[10] = {
    706ULL,
    31541ULL,
    1405874ULL,
    62693809ULL,
    2795479074ULL,
    124651925557ULL,
    5558261214722ULL,
    247844696877249ULL,
    11051471811217570ULL,
    492788608567408661ULL
};

/* Compute a(n) for n>=1 by the recurrence.  The function works
   for any idx_t (including very large n) and any seq_t (arbitrary
   precision or fixed width). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n <= 0 */

    /* Return the first 10 known values directly. */
    if (n <= 10) return init_vals[n - 1];

    /* Sliding window of the last 10 computed terms:
       window[0] = a(k-1), window[1] = a(k-2), …, window[9] = a(k-10).
       Initially we need the window for k=11, so we fill it with
       a(10)…a(1) in reverse order. */
    seq_t window[10];
    for (int i = 0; i < 10; ++i) {
        window[i] = init_vals[9 - i];
    }

    for (idx_t k = 11; k <= n; ++k) {
        seq_t pos = 0;   /* sum of positive‑coeff contributions */
        seq_t neg = 0;   /* sum of |negative‑coeff| contributions */

        for (int i = 0; i < 10; ++i) {
            if (coeffs[i] >= 0) {
                pos += window[i] * (seq_t)coeffs[i];
            } else {
                neg += window[i] * (seq_t)(-coeffs[i]);
            }
        }
        seq_t next = pos - neg;   /* a(k) = sum(coeff_i * window[i]) */

        /* Shift window rightwards and insert new term at front. */
        for (int i = 9; i > 0; --i) {
            window[i] = window[i - 1];
        }
        window[0] = next;
    }

    return window[0];   /* a(n) is now in window[0] */
}
