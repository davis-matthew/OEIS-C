/*
 * A228459  Number of arrays of maxima of three adjacent elements of some
 *          length n+2 0..6 array.
 * https://oeis.org/A228459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the empirical linear recurrence (order 19)
 *            a(n) = 7*a(n-1) -21*a(n-2) +35*a(n-3) -14*a(n-4) -14*a(n-5)
 *            +98*a(n-6) -6*a(n-7) +63*a(n-8) +151*a(n-9) +59*a(n-10)
 *            +107*a(n-11) +124*a(n-12) +60*a(n-13) +56*a(n-14) +44*a(n-15)
 *            +17*a(n-16) +9*a(n-17) +5*a(n-18) +a(n-19) Initial values...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t init_vals[19] = {
    7ULL,
    49ULL,
    252ULL,
    1036ULL,
    3703ULL,
    12453ULL,
    42386ULL,
    151882ULL,
    567835ULL,
    2147149ULL,
    8034340ULL,
    29610140ULL,
    108114007ULL,
    394245755ULL,
    1442791435ULL,
    5301606775ULL,
    19520651849ULL,
    71865418075ULL,
    264265863322ULL
};

static const int coeffs[19] = {
    7,   // a(n-1)
   -21,  // a(n-2)
    35,  // a(n-3)
   -14,  // a(n-4)
   -14,  // a(n-5)
    98,  // a(n-6)
   -6,   // a(n-7)
    63,  // a(n-8)
   151,  // a(n-9)
    59,  // a(n-10)
   107,  // a(n-11)
   124,  // a(n-12)
    60,  // a(n-13)
    56,  // a(n-14)
    44,  // a(n-15)
    17,  // a(n-16)
     9,  // a(n-17)
     5,  // a(n-18)
     1   // a(n-19)
};

seq_t a(idx_t n)
{
    if (n == 0)     /* offset is 1, so n >= 1 for valid usage */
        return 0;

    if (n <= 19) {
        /* idx_t count is unsigned, convert to size_t for array indexing */
        return init_vals[n - 1];
    }

    /* Utility array holding the previous 19 values:
       prev[0] = a(k-1), prev[1] = a(k-2), ..., prev[18] = a(k-19)
       Initialized for k = 20 (i.e. we are ready to compute a(20)).      */
    seq_t prev[19];
    for (int i = 0; i < 19; ++i)
        prev[i] = init_vals[18 - i];   /* a(19), a(18), ..., a(1) */

    /* Compute terms a(20) .. a(n) iteratively */
    for (idx_t k = 20; k <= n; ++k) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (int j = 0; j < 19; ++j) {
            if (coeffs[j] >= 0) {
                pos_sum += (seq_t)coeffs[j] * prev[j];
            } else {
                neg_sum += (seq_t)(-coeffs[j]) * prev[j];
            }
        }
        seq_t next = pos_sum;
        next -= neg_sum;   /* safe: pos_sum >= neg_sum for all k */

        /* Shift the window: drop oldest value, prepend new value. */
        for (int j = 18; j > 0; --j)
            prev[j] = prev[j - 1];
        prev[0] = next;
    }

    return prev[0];   /* a(n) */
}
