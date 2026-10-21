/*
 * A092821  Table read by rows: T(n,k)= z (z') or product of z with its
 *          complex conjugate, with z=Sum[binomial[n,t] I^t, {t,0,k}].
 * https://oeis.org/A092821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = | Σ_{t=0}^{k} C(r,t) i^t |^2 where (r,k) is obtained from
 *            n * by reading the triangular array T(r,k). The row index r is *
 *            the unique integer with n ≥ r(r+1)/2 and n < (r+1)(r+2)/2, * and
 *            k = n – r(r+1)/2. The value T(r,k) is the squared * magnitude of
 *            the complex partial sum of the binomi...
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* ----------- find r s.t. n < (r+1)(r+2)/2 ------------------- */
    /* compute s = floor(sqrt(8*n + 1)) using an integer search on __int128 */
    __int128 value = (__int128)8 * n + 1;          /* 8*n + 1 fits in 128 bits */
    unsigned long long lo = 0, hi = 1;
    while ((__int128)hi * hi <= value) hi <<= 1;   /* grow hi until hi^2 > value */
    while (lo < hi) {
        unsigned long long mid = (lo + hi + 1) >> 1;
        if ((__int128)mid * mid <= value) lo = mid;
        else                               hi = mid - 1;
    }
    unsigned long long sqrt_val = lo;              /* floor(sqrt(8*n+1))        */
    idx_t r = (idx_t)((sqrt_val - 1ULL) >> 1);     /* r = (sqrt - 1)/2          */

    /* ------------------- compute column k ----------------------- */
    unsigned __int128 prev = (unsigned __int128)r * (r + 1) / 2; /* terms before row r */
    idx_t k = n - (idx_t)prev;                        /* 0 ≤ k ≤ r              */

    /* ------------------- partial binomial sum ------------------- */
    seq_t bin     = 1;   /* C(r,0) */
    seq_t real    = 0;
    seq_t imag    = 0;

    for (idx_t t = 0; t <= k; ++t) {
        switch (t & 3U) {                /* power of i: 0→real+, 1→imag+, 2→real−, 3→imag− */
            case 0: real += bin;       break;
            case 1: imag += bin;       break;
            case 2: real -= bin;       break;
            case 3: imag -= bin;       break;
        }
        if (t == k) break;              /* last term processed */

        /* update binomial coefficient for next t: C(r,t+1) = C(r,t) * (r - t) / (t + 1) */
        bin = bin * (seq_t)( (idx_t)r - t );
        bin = bin / (seq_t)(t + 1);
    }

    /* --------------------- result ------------------------------- */
    return real * real + imag * imag;
}
